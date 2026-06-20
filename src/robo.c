#include "robo.h"
#include "simulacao.h"

int robo_passo_ate(struct Simulacao *sim, Robo *r, Vec2 alvo)
{
    /* TODO: dar um passo em direcao a 'alvo' respeitando paredes e ocupacao
     * das celulas (mapa_tenta_mover). Retornar 1 quando o robo ja estiver
     * sobre/adjacente ao alvo, 0 caso ainda precise andar.
     * Sugestao: comece com um passo guloso; evolua para BFS para desviar de
     * obstaculos e de outros robos. */
    (void)sim;
    (void)r;
    (void)alvo;
    return 0;
}
