/*
 * tests/teste_bfs.c — prova que robo_passo_ate (BFS) desvia de obstáculo em U.
 *
 * Mapa 7x7 com um U de paredes que bloqueia o caminho direto de S para T:
 *
 *   col: 0 1 2 3 4 5 6
 *   row 0: . . . . . . .
 *   row 1: . S . . . . .
 *   row 2: . # # # # . .
 *   row 3: . . . . # . .
 *   row 4: . . . T # . .
 *   row 5: . . . . . . .
 *   row 6: . . . . . . .
 *
 *   S = (1,1)   T = (3,4)
 *   Paredes: (1,2)(2,2)(3,2)(4,2)(4,3)(4,4)
 *
 * O caminho direto (descer/direita) é bloqueado pelo U. O BFS deve contornar
 * pela esquerda ou pela parte inferior do U. Um algoritmo guloso (greedy)
 * ficaria preso tentando ir direito/para baixo.
 *
 * Compilar e executar: make teste-bfs
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "mapa.h"
#include "robo.h"
#include "simulacao.h"
#include "comum.h"

#define MAP_W 7
#define MAP_H 7
#define MAX_PASSOS 60

static void imprimir(const Mapa *m, Vec2 rpos, Vec2 alvo, int passo)
{
    printf("passo %2d  robo(%d,%d):\n", passo, rpos.x, rpos.y);
    for (int y = 0; y < m->altura; y++) {
        for (int x = 0; x < m->largura; x++) {
                if (x == rpos.x && y == rpos.y)  { putchar('R'); continue; }
            if (x == alvo.x && y == alvo.y)  { putchar('T'); continue; }
            putchar(m->celulas[y * m->largura + x].tipo == CELULA_PAREDE
                    ? '#' : '.');
        }
        putchar('\n');
    }
    putchar('\n');
}

int main(void)
{
    printf("=== Teste BFS — obstaculo em U ===\n\n");

    /* Montar Simulacao minima: so o mapa importa para robo_passo_ate */
    Simulacao sim;
    memset(&sim, 0, sizeof(sim));
    assert(mapa_init(&sim.mapa, MAP_W, MAP_H) == 0);

    /* Paredes do U */
    static const int paredes[][2] = {
        {1,2}, {2,2}, {3,2}, {4,2},   /* barra superior */
        {4,3},                          /* perna direita  */
        {4,4},                          /* perna direita  */
    };
    for (size_t i = 0; i < sizeof(paredes) / sizeof(paredes[0]); i++) {
        int x = paredes[i][0], y = paredes[i][1];
        mapa_celula(&sim.mapa, x, y)->tipo = CELULA_PAREDE;
    }

    Vec2 inicio = { 1, 1 };
    Vec2 alvo   = { 3, 4 };

    /* Robo na posicao inicial */
    Robo robo;
    memset(&robo, 0, sizeof(robo));
    robo.id  = 1;
    robo.pos = inicio;
    mapa_celula(&sim.mapa, inicio.x, inicio.y)->ocupante = robo.id;

    printf("Mapa inicial (R=robo, T=alvo, #=parede):\n");
    imprimir(&sim.mapa, robo.pos, alvo, 0);

    /* Executar BFS passo a passo */
    int chegou = 0;
    int passos  = 0;

    while (passos < MAX_PASSOS) {
        int ret = robo_passo_ate(&sim, &robo, alvo);
        passos++;
        if (ret) {
            chegou = 1;
            break;
        }
    }

    printf("Mapa final:\n");
    imprimir(&sim.mapa, robo.pos, alvo, passos);

    printf("Posicao final : (%d,%d)\n", robo.pos.x, robo.pos.y);
    printf("Alvo          : (%d,%d)\n", alvo.x, alvo.y);
    printf("Distancia     : %d\n", distancia_manhattan(robo.pos, alvo));
    printf("Passos dados  : %d / %d\n\n", passos, MAX_PASSOS);

    assert(chegou
        && "BFS deve chegar ao alvo (ou ficar adjacente) antes de MAX_PASSOS");
    assert(distancia_manhattan(robo.pos, alvo) <= 1
        && "Robo deve estar sobre ou adjacente ao alvo ao retornar 1");
    assert(passos < MAX_PASSOS
        && "BFS nao deve precisar do limite maximo de passos para este mapa");

    printf("[OK] BFS desviou do U e chegou em %d passos.\n", passos);

    mapa_destroy(&sim.mapa);
    return 0;
}
