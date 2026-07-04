# Relatório Técnico — Centro de Distribuição Automatizado (IDP)

**Disciplina:** Programação Concorrente  
**Autora:** Helenna Massarra  

---

## 1. Organização das threads

O programa cria **N + M + 3 threads** ao total, onde N é o número de robôs coletores e M o de entregadores:

| Thread | Função | Quantidade | Argumento |
|--------|--------|:----------:|-----------|
| Gerador de pacotes | `thread_gerador` | 1 | `Simulacao *` |
| Esteira | `thread_esteira` | 1 | `Simulacao *` |
| Robô coletor | `thread_coletor` | N | `ArgRobo *` (ponteiros para `sim` e `sim.coletores[i]`) |
| Robô entregador | `thread_entregador` | M | `ArgRobo *` |
| Interface Raylib | `thread_interface` | 1 | `Simulacao *` |

Todas as threads são criadas em `main.c` com `pthread_create` e aguardadas com `pthread_join`. A janela Raylib é criada **dentro** de `thread_interface` (via `InitWindow`), pois a biblioteca exige que o contexto OpenGL seja criado e usado pela mesma thread — por isso `interface_init` em `main.c` apenas configura o nível de log, sem abrir janela.

O encerramento é coordenado pela flag `sim.rodando` (detalhada na seção 4): quando ela vai a zero, todos os laços de thread terminam naturalmente e os `pthread_join` em `main.c` retornam.

---

## 2. Recursos compartilhados

| Recurso | Tipo | Acessado por |
|---------|------|-------------|
| `Mapa.celulas[].ocupante` | grade de células | todos os robôs (leitura/escrita de posição) e interface (leitura) |
| `Esteira.posicoes[]` | buffer circular | robôs coletores (inserção), thread esteira (avanço), robôs entregadores (retirada), interface (leitura do contador) |
| `Estacao.cabeca/cauda/quantidade` | fila encadeada por estação | gerador (enfileira) e robôs coletores (desenfileira) |
| `Estatisticas` (`pacotes_gerados`, `pacotes_entregues`) | contadores inteiros | gerador e entregadores (escrita); interface (leitura) |
| `Simulacao.rodando` | flag de parada | qualquer thread pode escrever; todas leem |

---

## 3. Seções críticas identificadas

**Movimentação de robôs (`mapa.c`)**  
Dois robôs podem tentar se mover para a mesma célula simultaneamente. A seção crítica é a verificação e atualização de `celula->ocupante`: deve ser feita atomicamente para que apenas um robo ocupe cada célula. Implementado em `mapa_tenta_mover`, que adquire `Mapa.mutex`, verifica o destino, atualiza origem e destino e libera o mutex em um único lock.

**Inserção, avanço e retirada na esteira (`esteira.c`)**  
O vetor `posicoes[]` é lido e escrito por até três threads distintas:
- `esteira_tenta_inserir` (coletor) escreve em `posicoes[0]`;
- `esteira_avancar` (thread da esteira) desloca pacotes de `[i-1]` para `[i]`;
- `esteira_retirar_saida` (entregador) lê e anula `posicoes[tamanho-1]`.

Todas as três adquirem `Esteira.mutex` no início e liberam ao final.

**Filas de estações (`estacao.c`)**  
O gerador e múltiplos coletores disputam cada estação. `estacao_enfileirar` e `estacao_desenfileirar` adquirem `Estacao.mutex` para garantir que a lista encadeada e o contador `quantidade` são modificados atomicamente.

**Contadores de estatísticas (`simulacao.c`)**  
`stats_inc_gerados` e `stats_inc_entregues` adquirem `Estatisticas.mutex`. `stats_inc_entregues` verifica a condição de término **sob o mesmo lock** para que a chamada a `simulacao_parar` não seja disparada duas vezes.

---

## 4. Mecanismos de sincronização utilizados

### `pthread_mutex_t` — exclusão mútua por recurso

Cada recurso compartilhado possui seu próprio mutex (`Mapa.mutex`, `Esteira.mutex`, `Estacao.mutex` por estação, `Estatisticas.mutex`). Essa granularidade fina permite que threads que acessam recursos distintos avancem em paralelo.

A ordem de aquisição de locks segue uma hierarquia estrita: nenhuma thread adquire dois locks simultaneamente, eliminando o risco de deadlock por ordem de aquisição circular.

### `volatile sig_atomic_t rodando` — flag de término sem mutex

A flag `Simulacao.rodando` é declarada `volatile sig_atomic_t`. Isso é suficiente para a semântica de parada:
- `volatile` impede que o compilador cache o valor em registrador entre iterações do laço.
- `sig_atomic_t` garante que a leitura/escrita é atômica em nível de hardware em x86.
- O pior caso de corrida é que uma thread leia `1` um tick depois de outra ter escrito `0`, o que apenas faz ela executar mais um ciclo antes de encerrar — aceitável para esta simulação.

Usar um mutex para a flag adicionaria contenção desnecessária em todos os robôs a cada tick.

### Espera ativa com `dormir_ms` (busy-wait com sleep)

Nos pontos de espera (coletor aguardando espaço na esteira; entregador aguardando pacote na saída; robo bloqueado por outro), as threads dormem `passo_ms` e tentam novamente. Isso é preferível a um `pthread_cond_t` para este problema porque:
1. O período de espera já é determinado pelo ritmo da simulação (`passo_ms`).
2. Um `cond_wait` exigiria `pthread_cond_signal` em vários pontos dispersos, tornando a sincronização mais difícil de auditar.

---

## 5. Decisões de projeto

### Interface gráfica com Raylib (substituição de ncurses — bônus +10%)

O enunciado prevê ncurses como interface padrão e oferece bônus de até 10 pontos para grupos que substituam integralmente a interface por uma implementação em Raylib. Este projeto opta pela interface Raylib: o mapa e o painel lateral são desenhados em uma janela gráfica com cores e barra de progresso, proporcionando visualização mais clara da simulação. A lógica concorrente é idêntica à que seria usada com ncurses — a thread de interface captura um snapshot do estado (copiando `Mapa.celulas` sob `Mapa.mutex` e lendo contadores sob `Estatisticas.mutex`) e renderiza em seguida.

### Por que um mutex por recurso?

Um mutex global simplificaria a implementação, mas criaria um gargalo severo: a thread de interface (que lê o mapa a cada 100 ms) bloquearia todos os robôs durante a renderização. Com mutexes por recurso, cada robô bloqueia apenas a célula que está tentando ocupar, e a interface adquire `Mapa.mutex` por menos de um microsegundo (cópia de buffer) antes de liberar.

### Estratégia de geração de pacotes

O gerador usa `rand_r` com semente própria (derivada da semente global com XOR), garantindo que os sorteios de estação sejam independentes do posicionamento inicial dos robôs (que usa `rand()`). Isso evita que todos os pacotes caiam sempre nas mesmas estações.

### Estratégia de escolha de estação/despacho

Coletores escolhem a estação **não-vazia mais próxima** (distância de Manhattan). Isso é uma heurística gulosa: não é globalmente ótima, mas evita que todos os coletores disputem a mesma estação. A coleta real é feita por `estacao_desenfileirar` (atômica), que retorna `NULL` se outro coletor chegou primeiro — nesse caso o coletor simplesmente escolhe novamente sem penalidade.

Entregadores escolhem o **despacho mais próximo** da sua posição atual ao retirar cada pacote.

### BFS para navegação

Cada passo de cada robô recalcula o BFS a partir da posição atual, usando o estado do mapa **sem mutex** (leitura lock-free de `ocupante`). Dados levemente desatualizados apenas fazem o robô tentar um passo que falha em `mapa_tenta_mover` (atômico); no próximo ciclo o BFS recalcula com dados mais frescos. Isso elimina contenção no mutex do mapa durante o planejamento de rotas.

### Lógica `afastar_da_entrada`

Sem essa lógica, robôs coletores ociosos podiam se posicionar adjacentes à entrada da esteira e bloquear o único coletor carregado que precisava inserir seu pacote. A função `afastar_da_entrada` move o coletor ocioso para a célula vizinha mais distante da entrada, liberando o gargalo.

### Como o término é coordenado

Quando `stats_inc_entregues` detecta que `pacotes_entregues >= total_pacotes`, chama `simulacao_parar`, que escreve `rodando = 0`. Todas as threads verificam `simulacao_ativa` em seus laços principais e encerram na próxima iteração. Não há `pthread_cancel` nem sinais assíncronos; o encerramento é sempre cooperativo.

---

## 6. Guia de utilização

### Pré-requisitos (Ubuntu 24.04)

A interface usa Raylib, que precisa ser compilada do fonte (não há pacote `apt` no Ubuntu). O `Dockerfile` automatiza tudo; para compilar manualmente:

```bash
sudo apt update && sudo apt install -y build-essential git cmake \
    libx11-dev libxrandr-dev libxi-dev libxcursor-dev libxinerama-dev libgl1-mesa-dev

git clone --depth 1 --branch 5.5 https://github.com/raysan5/raylib.git /tmp/raylib
cd /tmp/raylib && cmake -B build -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --target install && sudo ldconfig
```

### Compilar

```bash
make
```

### Executar

```bash
./idp_sim 1    # Cenário Simples (18×12, 20 pacotes)
./idp_sim 2    # Cenário Intermediário (24×16, 40 pacotes)
./idp_sim 3    # Cenário Alta Concorrência (32×20, 80 pacotes)
./idp_sim 0    # Configuração dinâmica (lê parâmetros do usuário)
```

Pressione **`q`** para encerrar antecipadamente.

### Testes

```bash
make teste          # valida mapa, simulacao e operacoes atomicas
make teste-bfs      # prova que o BFS desvia de obstaculo em U
make teste-stress   # ciclo completo com threads reais (sem interface gráfica)
```

### Via container (qualquer sistema com podman)

A interface gráfica precisa de acesso ao servidor X11 do host:

```bash
podman build -t idp_sim .
xhost +local:                              # libera acesso ao X11 local
podman run --rm -it \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    idp_sim 2
```
