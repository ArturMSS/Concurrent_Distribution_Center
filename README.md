# Centro de Distribuição Automatizado (IDP) — Programação Concorrente

Simulação concorrente, em C, de um centro de distribuição de pacotes da
*Indústria de Deslocamento de Pacotes* (IDP). Pacotes são gerados em estações
de coleta, transportados por robôs até a esteira, percorrem a esteira e são
retirados por robôs entregadores que os levam aos pontos de despacho.

---

## Casos disponíveis

| Caso | Forma de seleção | Descrição |
|:----:|-----------------|-----------|
| **0** | `./idp_sim 0` | Configuração dinâmica — você define os parâmetros interativamente |
| **1** | `./idp_sim 1` | Simples — mapa 18×12, 2 coletores, 1 entregador, meta 20 pacotes |
| **2** | `./idp_sim 2` | Intermediário — mapa 24×16, 3 coletores, 2 entregadores, meta 40 pacotes |
| **3** | `./idp_sim 3` | Alta concorrência — mapa 32×20, 5 coletores, 4 entregadores, meta 80 pacotes |

Pressione **`q`** a qualquer momento para encerrar a simulação antes de atingir a meta.

---

## Início rápido (container Ubuntu 24.04 — recomendado)

A forma mais simples de rodar em qualquer máquina é usando um container
Ubuntu 24.04 via **podman** (rootless, sem daemon).

### Pré-requisito: configurar podman rootless

Necessário apenas uma vez por usuário:

```bash
# 1. Verificar se já existem faixas de UIDs/GIDs (provavelmente vazio)
grep $USER /etc/subuid /etc/subgid

# 2. Adicionar as faixas subordinadas
sudo usermod --add-subuids 100000-165535 --add-subgids 100000-165535 $USER

# 3. Fazer o podman reconhecer a mudança
podman system migrate

# 4. Verificar que funciona
podman pull ubuntu:24.04
```

### 1. Construir a imagem

Na raiz do repositório:

```bash
podman build -t idp_sim .
```

### 2. Rodar os cenários

A interface usa ncurses (terminal interativo), por isso é preciso passar `-it`:

```bash
# Caso 1 — Simples (padrão)
podman run --rm -it idp_sim 1

# Caso 2 — Intermediário
podman run --rm -it idp_sim 2

# Caso 3 — Alta concorrência
podman run --rm -it idp_sim 3
```

#### Caso 0 — Configuração dinâmica

O caso 0 lê os parâmetros do usuário via stdin **antes** de abrir o ncurses,
por isso é preciso usar `-it`:

```bash
podman run --rm -it idp_sim 0
```

Você verá um prompt interativo no terminal:

```
=== Configuracao dinamica ===
Pressione Enter para usar o valor entre colchetes.

Largura do mapa (colunas) [18]:
Altura do mapa (linhas)   [12]:
Robos coletores           [2]:
Robos entregadores        [1]:
Estacoes de coleta (P)    [3]:
Pontos de despacho (D)    [2]:
Obstaculos (#)            [4]:
Tamanho da esteira        [6]:
Total de pacotes (meta)   [20]:
Intervalo de geracao (ms) [1200]:
Duracao de um tick (ms)   [300]:
Semente aleatoria         [42]:
```

Pressione **Enter** em cada campo para aceitar o valor padrão, ou digite
o novo valor antes de pressionar Enter.

### 3. Rodar os testes dentro do container

```bash
# abre um shell com o código do host montado em /trabalho
podman run --rm -it -v "$(pwd)":/trabalho:Z idp_sim bash

# dentro do container:
make clean        # garante recompilação com o código montado
make
make teste        # valida fundação (mapa, simulação, operações atômicas)
make teste-bfs    # prova que o BFS desvia de obstáculo em U
```

> **Nota:** a flag `:Z` no volume é necessária em sistemas com SELinux
> (Fedora, RHEL, etc.) para o container ter permissão de leitura/escrita.

---

## Rodar diretamente no Ubuntu 24.04

### Pré-requisitos

```bash
sudo apt update
sudo apt install build-essential libncurses-dev
```

### Compilar

```bash
make
```

### Executar — todos os casos

```bash
# Caso 1 — Simples (padrão quando nenhum argumento é passado)
./idp_sim
./idp_sim 1

# Caso 2 — Intermediário
./idp_sim 2

# Caso 3 — Alta concorrência
./idp_sim 3

# Caso 0 — Configuração dinâmica (pede parâmetros no terminal)
./idp_sim 0
```

Via `make`:

```bash
make run          # compila e roda o caso 1
make run N=2      # compila e roda o caso 2
make run N=3      # compila e roda o caso 3
make run N=0      # compila e roda o caso 0 (configuração dinâmica)
```

### Testes

```bash
make teste        # valida mapa, simulação e operações atômicas
make teste-bfs    # prova que o BFS desvia de obstáculo em U
make teste-stress # teste de estresse com múltiplas threads
```

### Limpar

```bash
make clean
```

---

## Parâmetros dos cenários estáticos

| # | Nome | Mapa | Coletores | Entregadores | Estações | Despachos | Obstáculos | Esteira | Meta |
|:-:|------|------|:---------:|:------------:|:--------:|:---------:|:----------:|:-------:|:----:|
| 1 | Simples | 18×12 | 2 | 1 | 3 | 2 | 4 | 6 | 20 |
| 2 | Intermediário | 24×16 | 3 | 2 | 4 | 3 | 12 | 8 | 40 |
| 3 | Alta concorrência | 32×20 | 5 | 4 | 6 | 4 | 24 | 12 | 80 |

---

## Legenda da interface

| Símbolo | Cor | Significado |
|---------|-----|-------------|
| `P` | amarelo | estação de coleta |
| `I` | magenta | entrada da esteira |
| `O` | magenta | saída da esteira |
| `D` | amarelo | ponto de despacho |
| `C` | verde | robô coletor |
| `E` | ciano | robô entregador |
| `#` | branco | parede / obstáculo |
| `.` | — | célula livre |

---

## Organização do código

```
include/   cabeçalhos (.h)
src/       implementações (.c)
tests/     testes manuais
cenarios/  descrição dos cenários
docs/      relatório técnico
Makefile
Dockerfile
```

| Módulo | Papel |
|--------|-------|
| `config.{h,c}` | parâmetros e cenários estáticos + configuração dinâmica (caso 0) |
| `mapa.{h,c}` | grade de células + ocupação (recurso compartilhado) |
| `esteira.{h,c}` | esteira (buffer limitado) + thread da esteira |
| `estacao.{h,c}` | estação de coleta com fila de pacotes |
| `pacote.{h,c}` | estrutura do pacote |
| `robo.{h,c}` | tipo `Robo` + motor BFS de movimento |
| `gerador.{h,c}` | thread do gerador de pacotes |
| `robo_coletor.{h,c}` | thread dos robôs coletores |
| `robo_entregador.{h,c}` | thread dos robôs entregadores |
| `interface.{h,c}` | interface ncurses (thread de exibição) |
| `simulacao.{h,c}` | estado global + estatísticas + ciclo de vida |
| `main.c` | criação e junção das threads |

---

## Arquitetura de threads e sincronização

**Threads** criadas por `main.c`:

| Thread | Função | Argumento |
|--------|--------|-----------|
| Gerador | `thread_gerador` | `Simulacao *` |
| Esteira | `thread_esteira` | `Simulacao *` |
| Robô Coletor × N | `thread_coletor` | `ArgRobo *` |
| Robô Entregador × M | `thread_entregador` | `ArgRobo *` |
| Interface | `thread_interface` | `Simulacao *` |

**Recursos compartilhados e sua proteção:**

| Recurso | Campo | Mutex |
|---------|-------|-------|
| Células do mapa | `Mapa.celulas[].ocupante` | `Mapa.mutex` |
| Esteira | `Esteira.posicoes` | `Esteira.mutex` |
| Fila de cada estação | `Estacao` (lista encadeada) | `Estacao.mutex` |
| Contadores | `Estatisticas` | `Estatisticas.mutex` |
| Flag de parada | `Simulacao.rodando` | `volatile sig_atomic_t` |

**Garantia de exclusão mútua no mapa:**
`mapa_tenta_ocupar`, `mapa_liberar` e `mapa_tenta_mover` adquirem
`Mapa.mutex` em cada operação. `mapa_tenta_mover` verifica e altera origem e
destino em um único lock, impedindo que dois robôs ocupem a mesma célula.
