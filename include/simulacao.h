#ifndef SIMULACAO_H
#define SIMULACAO_H

#include <pthread.h>
#include <signal.h>
#include "config.h"
#include "mapa.h"
#include "esteira.h"
#include "estacao.h"
#include "robo.h"

/*
 * simulacao.h - Estado GLOBAL compartilhado da simulacao.
 *
 * Um unico objeto Simulacao reune todos os recursos compartilhados. Cada
 * thread recebe um ponteiro para ele. Os mutexes ficam encapsulados em cada
 * recurso (mapa, esteira, estacao, estatisticas).
 *
 * Organizacao das threads (ver os respectivos .c):
 *   - 1 thread Gerador de Pacotes        (gerador.c)
 *   - 1 thread Esteira                    (esteira.c -> thread_esteira)
 *   - N threads Robo Coletor              (robo_coletor.c)
 *   - M threads Robo Entregador           (robo_entregador.c)
 *   - 1 thread Interface (Raylib)          (interface.c)
 */

/* Estatisticas/contadores globais exibidos pela interface. */
typedef struct {
    int pacotes_gerados;
    int pacotes_entregues;
    int total_pacotes;      /* meta para encerrar a simulacao */
    double inicio;          /* instante de inicio (para o cronometro) */
    pthread_mutex_t mutex;
} Estatisticas;

typedef struct Simulacao {
    Config cfg;

    Mapa mapa;
    Esteira esteira;
    Estacao *estacoes;      /* vetor [cfg.num_estacoes] */
    Vec2 *despachos;        /* vetor [cfg.num_despachos] */
    Robo *coletores;        /* vetor [cfg.num_coletores] */
    Robo *entregadores;     /* vetor [cfg.num_entregadores] */

    Vec2 entrada;           /* posicao da entrada da esteira (in) */
    Vec2 saida;             /* posicao da saida da esteira (out) */

    Estatisticas stats;

    /* Flag de execucao. sig_atomic_t + volatile para leitura/escrita segura
     * entre threads e tratador de sinal. Quando vira 0, todas as threads
     * encerram seus lacos. */
    volatile sig_atomic_t rodando;
} Simulacao;

/* Aloca e inicializa todos os recursos a partir da configuracao, monta o
 * mapa (estacoes, despachos, entrada, saida, obstaculos) e posiciona os
 * robos. Retorna 0 em sucesso. */
int simulacao_init(Simulacao *sim, Config cfg);

/* Libera todos os recursos da simulacao. */
void simulacao_destroy(Simulacao *sim);

/* Verdadeiro enquanto a simulacao deve continuar rodando. */
int simulacao_ativa(Simulacao *sim);

/* Sinaliza o encerramento da simulacao (para todas as threads). */
void simulacao_parar(Simulacao *sim);

/* --- Acesso sincronizado as estatisticas --- */
void stats_inc_gerados(Simulacao *sim);
void stats_inc_entregues(Simulacao *sim);
int  stats_pacotes_entregues(Simulacao *sim);

/* Total de pacotes aguardando coleta somando todas as estacoes. */
int simulacao_pacotes_aguardando(Simulacao *sim);

#endif /* SIMULACAO_H */
