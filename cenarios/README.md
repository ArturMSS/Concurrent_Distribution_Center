# Cenários estáticos

Os cenários são definidos em código, em [`src/config.c`](../src/config.c),
e selecionados pelo argumento de linha de comando (`./idp_sim N`).

| #  | Nome                  | Mapa   | Coletores | Entregadores | Estações | Despachos | Obstác. | Esteira | Pacotes |
|----|-----------------------|--------|-----------|--------------|----------|-----------|---------|---------|---------|
| 1  | Simples               | 18×12  | 2         | 1            | 3        | 2         | 4       | 6       | 20      |
| 2  | Intermediário         | 24×16  | 3         | 2            | 4        | 3         | 12      | 8       | 40      |
| 3  | Alta concorrência     | 32×20  | 5         | 4            | 6        | 4         | 24      | 12      | 80      |

Para criar um novo cenário, adicione um `case` em
`config_carregar_cenario()` e atualize `NUM_CENARIOS` em
[`include/config.h`](../include/config.h).

> Alternativa: implementar a **configuração dinâmica**
> (`config_carregar_dinamico`), lendo os parâmetros do usuário em tempo de
> execução — o que vale até 100% desta etapa, segundo o enunciado.
