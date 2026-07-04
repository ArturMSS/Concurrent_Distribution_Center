#ifndef ROBO_H
#define ROBO_H

#include "comum.h"
#include "pacote.h"

/*
 * robo.h - Estrutura comum aos robos coletores e entregadores, e funcoes
 * auxiliares de movimentacao.
 *
 * As threads especificas de cada tipo de robo estao em robo_coletor.c e
 * robo_entregador.c. Aqui ficam o tipo de dado e o "motor" de movimento
 * que respeita a ocupacao das celulas do mapa.
 */

typedef enum {
    ROBO_COLETOR,
    ROBO_ENTREGADOR
} TipoRobo;

typedef enum {
    ROBO_OCIOSO,      /* sem tarefa */
    ROBO_INDO,        /* deslocando-se ate um alvo */
    ROBO_CARREGADO    /* transportando um pacote */
} EstadoRobo;

typedef struct {
    int id;           /* id global unico (usado tambem como 'ocupante' no mapa) */
    TipoRobo tipo;
    Vec2 pos;         /* posicao atual no mapa */
    EstadoRobo estado;
    Pacote *carga;    /* pacote transportado, ou NULL */
} Robo;

/* Forward declaration para evitar dependencia circular com simulacao.h. */
struct Simulacao;

/* Argumento passado para as threads de robo: a simulacao e o robo especifico
 * que aquela thread controla. */
typedef struct {
    struct Simulacao *sim;
    Robo *robo;
} ArgRobo;

/* Da um passo em direcao a 'alvo', respeitando paredes e ocupacao das
 * celulas (movimento atomico via mapa_tenta_mover). Retorna 1 quando o robo
 * ja esta ADJACENTE (ou sobre) o alvo, indicando que chegou; 0 caso ainda
 * precise andar.
*/
int robo_passo_ate(struct Simulacao *sim, Robo *r, Vec2 alvo);

#endif /* ROBO_H */
