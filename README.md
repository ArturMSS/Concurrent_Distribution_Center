# Centro de Distribuição Automatizado (IDP) — Programação Concorrente

Simulação concorrente, em C, de um centro de distribuição de pacotes da
*Indústria de Deslocamento de Pacotes* (IDP). Pacotes são gerados em estações
de coleta, transportados por robôs até a esteira, percorrem a esteira e são
retirados por robôs entregadores que os levam aos pontos de despacho.

> **Status:** esqueleto. Os headers definem a arquitetura (structs, recursos
> compartilhados com seus mutexes e protótipos) e os `.c` têm apenas os stubs
> das funções, com comentários `// TODO` indicando o que cada uma deve fazer.
> Toda a lógica ainda deve ser implementada. 

## Compilação

Requisitos (Ubuntu 24.04 / similar ao laboratório):

```bash
sudo apt install build-essential libncurses-dev
```

Compilar:

```bash
make
```

## Execução

```bash
./idp_sim         # cenário 1 (padrão)
./idp_sim 2       # cenário 2
./idp_sim 3       # cenário 3

make run          # compila e roda o cenário 1
make run N=3      # compila e roda o cenário 3
```

Pressione **`q`** para encerrar. A simulação também termina sozinha quando a
meta de pacotes entregues (`total_pacotes` do cenário) é atingida.

### Legenda da interface

| Símbolo | Significado            |
|---------|------------------------|
| `P`     | estação de coleta      |
| `I`     | entrada da esteira (in)|
| `O`     | saída da esteira (out) |
| `D`     | ponto de despacho      |
| `C`     | robô coletor           |
| `E`     | robô entregador        |
| `#`     | parede / obstáculo     |
| `.`     | célula livre           |

## Organização do código

```
include/   cabeçalhos (.h)
src/       implementações (.c)
cenarios/  descrição dos cenários estáticos
docs/      relatório técnico (a escrever)
Makefile
```

| Módulo                         | Papel                                              |
|--------------------------------|----------------------------------------------------|
| `config.{h,c}`                 | parâmetros e cenários estáticos                    |
| `mapa.{h,c}`                   | grade de células + ocupação (recurso compartilhado)|
| `esteira.{h,c}`                | esteira (buffer limitado) + thread da esteira      |
| `estacao.{h,c}`                | estação de coleta com fila de pacotes              |
| `pacote.{h,c}`                 | estrutura do pacote                                |
| `robo.{h,c}`                   | tipo `Robo` + motor de movimento                   |
| `gerador.{h,c}`                | thread do gerador de pacotes                       |
| `robo_coletor.{h,c}`           | thread dos robôs coletores                         |
| `robo_entregador.{h,c}`        | thread dos robôs entregadores                      |
| `interface.{h,c}`              | interface ncurses (thread de exibição)             |
| `simulacao.{h,c}`              | estado global + estatísticas + ciclo de vida       |
| `main.c`                       | criação e junção das threads                       |

## Mapa de threads e recursos compartilhados

**Threads** (uma por elemento concorrente):
- 1 × Gerador de Pacotes
- 1 × Esteira
- N × Robô Coletor
- M × Robô Entregador
- 1 × Interface

**Recursos compartilhados e sua proteção:**

| Recurso             | Onde                       | Sincronização          |
|---------------------|----------------------------|------------------------|
| Células do mapa     | `Mapa.celulas` / `ocupante`| `Mapa.mutex`           |
| Esteira             | `Esteira.posicoes`         | `Esteira.mutex`        |
| Fila de cada estação| `Estacao` (lista)          | `Estacao.mutex`        |
| Contadores/estatíst.| `Estatisticas`             | `Estatisticas.mutex`   |
| Flag de execução    | `Simulacao.rodando`        | `volatile sig_atomic_t`|

## Próximos passos sugeridos (TODO)

1. Substituir o movimento "guloso" de `robo.c` por busca de caminho (BFS) que
   desvie de paredes e de outros robôs.
2. Trocar a espera ativa do coletor na entrada da esteira por variável de
   condição (`pthread_cond_t`).
3. Implementar a configuração dinâmica em `config_carregar_dinamico`.
4. Refinar estratégias de escolha de estação/despacho e geração de pacotes.
5. (Bônus) Reescrever a interface usando **raylib** em vez de ncurses.
