#ifndef ESTACAO_H
#define ESTACAO_H

#include <pthread.h>
#include "comum.h"
#include "pacote.h"

/*
 * estacao.h - Estacao geradora de pacotes (posicao P do mapa).
 *
 * Cada estacao mantem uma FILA de pacotes aguardando coleta. E um RECURSO
 * COMPARTILHADO: o gerador enfileira pacotes e varios robos coletores podem
 * tentar retirar pacotes da mesma estacao simultaneamente. A fila e
 * protegida pelo mutex da estacao.
 */

typedef struct NoPacote {
    Pacote *pacote;
    struct NoPacote *prox;
} NoPacote;

typedef struct {
    Vec2 pos;               /* posicao da estacao no mapa */
    NoPacote *cabeca;       /* inicio da fila */
    NoPacote *cauda;        /* fim da fila */
    int quantidade;         /* pacotes aguardando coleta */
    pthread_mutex_t mutex;
} Estacao;

/* Inicializa a estacao vazia na posicao 'pos'. Retorna 0 em sucesso. */
int estacao_init(Estacao *est, Vec2 pos);

/* Libera os recursos da estacao (e os pacotes ainda na fila). */
void estacao_destroy(Estacao *est);

/* Enfileira um pacote (chamado pelo gerador). Operacao atomica. */
void estacao_enfileirar(Estacao *est, Pacote *p);

/* Retira o primeiro pacote da fila, ou NULL se vazia. Operacao atomica. */
Pacote *estacao_desenfileirar(Estacao *est);

/* Quantidade de pacotes aguardando coleta nesta estacao. */
int estacao_quantidade(Estacao *est);

#endif /* ESTACAO_H */
