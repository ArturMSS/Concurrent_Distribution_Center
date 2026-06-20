#ifndef ESTEIRA_H
#define ESTEIRA_H

#include <pthread.h>
#include "pacote.h"

/*
 * esteira.h - Esteira transportadora.
 *
 * Buffer LIMITADO de posicoes discretas. posicoes[0] e a ENTRADA (in) e
 * posicoes[tamanho-1] e a SAIDA (out). A cada passo da simulacao os pacotes
 * avancam uma posicao em direcao a saida, desde que a proxima esteja livre.
 *
 * E um RECURSO COMPARTILHADO classico (produtor/consumidor com buffer
 * limitado): robos coletores inserem na entrada, a thread da esteira faz os
 * pacotes avancarem, e robos entregadores retiram da saida. Tudo protegido
 * pelo mutex da esteira.
 */

typedef struct {
    int tamanho;
    Pacote **posicoes;      /* vetor de ponteiros; NULL = posicao vazia */
    int ocupadas;           /* quantos pacotes estao na esteira */
    pthread_mutex_t mutex;
    /* TODO (opcional): pthread_cond_t para bloquear o coletor enquanto a
     * entrada estiver ocupada, em vez de usar espera ativa (retry). */
} Esteira;

/* Inicializa a esteira vazia com 'tamanho' posicoes. Retorna 0 em sucesso. */
int esteira_init(Esteira *e, int tamanho);

/* Libera os recursos da esteira. */
void esteira_destroy(Esteira *e);

/* Tenta inserir 'p' na entrada (posicoes[0]).
 * Retorna 1 se inseriu, 0 se a entrada estava ocupada. */
int esteira_tenta_inserir(Esteira *e, Pacote *p);

/* Avanca todos os pacotes uma posicao em direcao a saida (um "tick").
 * O pacote na saida permanece ate ser retirado por um entregador. */
void esteira_avancar(Esteira *e);

/* Retira e retorna o pacote da saida (posicoes[tamanho-1]).
 * Retorna NULL se a saida estiver vazia. */
Pacote *esteira_retirar_saida(Esteira *e);

/* Quantidade de pacotes atualmente na esteira. */
int esteira_ocupadas(Esteira *e);

/* Thread da esteira. Recebe um (Simulacao *) e, a cada passo, faz os
 * pacotes avancarem uma posicao em direcao a saida. */
void *thread_esteira(void *arg);

#endif /* ESTEIRA_H */
