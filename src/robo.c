#include "robo.h"
#include "simulacao.h"
#include "mapa.h"
#include "comum.h"
#include <string.h>

/*
 * BFS no grid 4-vizinhos para encontrar o menor caminho de 'origem' ate uma
 * celula sobre ou adjacente a 'alvo'.
 *
 * Bloqueios:
 *   - CELULA_PAREDE   -> bloqueio fixo (nunca entra na fila).
 *   - celula ocupada por outro robo -> bloqueio temporario (evitada, mas o
 *     alvo em si e sempre enfileirado mesmo se ocupado, pois queremos chegar
 *     perto dele).
 *
 * Leitura sem mutex: intencional — 'tipo' nunca muda apos init e dados de
 * 'ocupante' levemente desatualizados so fazem o robo tentar um passo que
 * falha em mapa_tenta_mover (atomico); na proxima chamada o BFS recalcula.
 *
 * Retorna o INDICE (y*W+x) do primeiro passo a dar a partir de 'origem',
 * ou -1 se nao houver caminho disponivel agora.
 */

#define MAPA_MAX_CELULAS 1024   /* >= 32*20 = 640 (maior cenario) */

static const int DX[4] = { 1, -1,  0,  0 };
static const int DY[4] = { 0,  0,  1, -1 };

static int bfs_proximo_passo(const Mapa *m, Vec2 origem, Vec2 alvo,
                              int id_robo)
{
    int W = m->largura;
    int H = m->altura;
    int total = W * H;

    int  prev[MAPA_MAX_CELULAS];
    char vis [MAPA_MAX_CELULAS];
    int  fila[MAPA_MAX_CELULAS];

    memset(vis,  0,   (size_t)total);
    memset(prev, -1,  (size_t)total * sizeof(int));

    int head = 0, tail = 0;
    int origem_idx = origem.y * W + origem.x;

    fila[tail++]     = origem_idx;
    vis[origem_idx]  = 1;

    while (head < tail) {
        int cur = fila[head++];
        int cx  = cur % W;
        int cy  = cur / W;
        Vec2 cur_pos = { cx, cy };

        /* Chegamos ao alvo ou a uma celula adjacente: reconstruir caminho */
        if (distancia_manhattan(cur_pos, alvo) <= 1) {
            int node = cur;
            while (prev[node] != origem_idx && prev[node] != -1)
                node = prev[node];
            /* 'node' e o filho direto da origem na menor rota */
            return (prev[node] == origem_idx) ? node : -1;
        }

        for (int d = 0; d < 4; d++) {
            int nx = cx + DX[d];
            int ny = cy + DY[d];
            if (nx < 0 || nx >= W || ny < 0 || ny >= H) continue;
            int nidx = ny * W + nx;
            if (vis[nidx]) continue;

            const Celula *c = &m->celulas[nidx];

            /* Parede: bloqueio fixo */
            if (c->tipo == CELULA_PAREDE) { vis[nidx] = 1; continue; }

            /* Outro robo: bloqueio temporario (exceto no proprio alvo) */
            Vec2 np = { nx, ny };
            if (c->ocupante != SEM_OCUPANTE && c->ocupante != id_robo
                    && distancia_manhattan(np, alvo) > 0) {
                vis[nidx] = 1;
                continue;
            }

            vis[nidx]  = 1;
            prev[nidx] = cur;
            if (tail < MAPA_MAX_CELULAS)
                fila[tail++] = nidx;
        }
    }

    return -1;  /* sem caminho disponivel agora */
}

int robo_passo_ate(struct Simulacao *sim, Robo *r, Vec2 alvo)
{
    /* Ja esta sobre o alvo ou adjacente: sinaliza chegada */
    if (distancia_manhattan(r->pos, alvo) <= 1)
        return 1;

    int W = sim->mapa.largura;

    int proximo_idx = bfs_proximo_passo(&sim->mapa, r->pos, alvo, r->id);
    if (proximo_idx < 0)
        return 0;   /* caminho bloqueado agora; tentar no proximo ciclo */

    Vec2 proximo = { proximo_idx % W, proximo_idx / W };

    /* Movimento atomico: pode falhar se outro robo chegou primeiro */
    if (!mapa_tenta_mover(&sim->mapa, r->pos, proximo, r->id))
        return 0;

    r->pos = proximo;
    return distancia_manhattan(r->pos, alvo) <= 1;
}
