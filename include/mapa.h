#ifndef MAPA_H
#define MAPA_H

#include <pthread.h>
#include "comum.h"

/*
 * mapa.h - Grade de celulas do ambiente.
 *
 * Cada celula tem um TIPO (terreno estatico) e um OCUPANTE (entidade que
 * esta sobre ela neste instante). O mapa e um RECURSO COMPARTILHADO: varios
 * robos consultam e alteram a ocupacao das celulas concorrentemente, por
 * isso todas as operacoes que mexem em 'ocupante' passam pelo mutex.
 *
 * Regra do enunciado: duas entidades NAO podem ocupar a mesma celula.
 */

typedef enum {
    CELULA_LIVRE,     /* espaco livre por onde robos andam */
    CELULA_PAREDE,    /* obstaculo intransponivel */
    CELULA_ESTACAO,   /* estacao geradora de pacotes (P) */
    CELULA_ENTRADA,   /* entrada da esteira (in) */
    CELULA_SAIDA,     /* saida da esteira (out) */
    CELULA_DESPACHO   /* ponto de despacho (D) */
} TipoCelula;

#define SEM_OCUPANTE (-1)

typedef struct {
    TipoCelula tipo;
    int ocupante;     /* id da entidade que ocupa, ou SEM_OCUPANTE */
} Celula;

typedef struct {
    int largura;
    int altura;
    Celula *celulas;        /* vetor largura*altura, indexado por y*largura + x */
    pthread_mutex_t mutex;  /* protege o campo 'ocupante' das celulas */
} Mapa;

/* Inicializa o mapa com todas as celulas livres. Retorna 0 em sucesso. */
int mapa_init(Mapa *m, int largura, int altura);

/* Libera os recursos do mapa (memoria e mutex). */
void mapa_destroy(Mapa *m);

/* Ponteiro para a celula (x, y). Retorna NULL se fora dos limites. */
Celula *mapa_celula(Mapa *m, int x, int y);

/* Verdadeiro se (x, y) esta dentro dos limites do mapa. */
int mapa_valido(const Mapa *m, int x, int y);

/* Tenta marcar (x, y) como ocupada por 'id'. Retorna 1 se conseguiu,
 * 0 se a celula estava ocupada ou e parede. Operacao atomica (mutex). */
int mapa_tenta_ocupar(Mapa *m, int x, int y, int id);

/* Libera a ocupacao da celula (x, y). Operacao atomica (mutex). */
void mapa_liberar(Mapa *m, int x, int y);

/* Move atomicamente o ocupante 'id' de 'de' para 'para'.
 * Retorna 1 se o destino estava livre e o movimento ocorreu, 0 caso
 * contrario. Garante que nenhuma celula fique com dois ocupantes. */
int mapa_tenta_mover(Mapa *m, Vec2 de, Vec2 para, int id);

#endif /* MAPA_H */
