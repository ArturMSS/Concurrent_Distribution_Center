/*
 * tests/teste_stress.c — valida o ciclo completo de entrega com threads reais.
 *
 * Sobe todas as threads operacionais (gerador, esteira, N coletores,
 * M entregadores) com uma configuracao minima e sem interface ncurses.
 * Uma thread de timeout encerra a simulacao se ela nao terminar dentro do
 * prazo, sinalizando deadlock ou livelock.
 *
 * Compilar e executar: make teste-stress
 */

#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "config.h"
#include "simulacao.h"
#include "gerador.h"
#include "esteira.h"
#include "robo_coletor.h"
#include "robo_entregador.h"
#include "comum.h"

/* Quanto tempo maximo o teste pode levar (ms). Com passo_ms=100 e 10 pacotes,
 * o caminho mais longo razoavel e << 30 s. */
#define TIMEOUT_MS 30000

typedef struct {
    Simulacao *sim;
    int        expirou;   /* 1 se o timeout disparou antes do termino */
} ArgTimeout;

static void *thread_timeout(void *arg)
{
    ArgTimeout *a = (ArgTimeout *)arg;
    int restante = TIMEOUT_MS;
    /* Polling de 100 ms: sai cedo se a simulacao encerrar antes do prazo */
    while (restante > 0 && simulacao_ativa(a->sim)) {
        dormir_ms(100);
        restante -= 100;
    }
    if (simulacao_ativa(a->sim)) {
        fprintf(stderr,
            "\n[FALHA] timeout de %d ms atingido — possivel deadlock/livelock.\n",
            TIMEOUT_MS);
        a->expirou = 1;
        simulacao_parar(a->sim);
    }
    return NULL;
}

int main(void)
{
    printf("=== Teste Stress — ciclo completo com threads ===\n\n");

    /* Configuracao minima: mapa pequeno, poucos pacotes, ticks rapidos */
    Config cfg;
    cfg.largura              = 14;
    cfg.altura               = 10;
    cfg.num_coletores        = 2;
    cfg.num_entregadores     = 2;
    cfg.num_estacoes         = 3;
    cfg.num_despachos        = 2;
    cfg.num_obstaculos       = 6;
    cfg.tamanho_esteira      = 5;
    cfg.total_pacotes        = 10;
    cfg.intervalo_geracao_ms = 80;
    cfg.passo_ms             = 40;
    cfg.seed                 = 7u;
    snprintf(cfg.nome, sizeof(cfg.nome), "Stress-minimo");

    Simulacao sim;
    int rc = simulacao_init(&sim, cfg);
    assert(rc == 0 && "simulacao_init deve retornar 0");

    printf("Config: %s  mapa %dx%d  meta=%d pacotes  timeout=%d ms\n\n",
           cfg.nome, cfg.largura, cfg.altura, cfg.total_pacotes, TIMEOUT_MS);

    /* Thread de timeout — monitora prazo maximo */
    ArgTimeout at = { &sim, 0 };
    pthread_t t_timeout;
    pthread_create(&t_timeout, NULL, thread_timeout, &at);

    /* Argumentos individuais para cada robo */
    ArgRobo args_col[cfg.num_coletores];
    ArgRobo args_ent[cfg.num_entregadores];
    for (int i = 0; i < cfg.num_coletores; i++) {
        args_col[i].sim  = &sim;
        args_col[i].robo = &sim.coletores[i];
    }
    for (int i = 0; i < cfg.num_entregadores; i++) {
        args_ent[i].sim  = &sim;
        args_ent[i].robo = &sim.entregadores[i];
    }

    /* Criar threads operacionais (sem interface ncurses) */
    pthread_t t_gerador, t_esteira;
    pthread_t t_col[cfg.num_coletores];
    pthread_t t_ent[cfg.num_entregadores];

    pthread_create(&t_gerador, NULL, thread_gerador, &sim);
    pthread_create(&t_esteira, NULL, thread_esteira, &sim);
    for (int i = 0; i < cfg.num_coletores; i++)
        pthread_create(&t_col[i], NULL, thread_coletor, &args_col[i]);
    for (int i = 0; i < cfg.num_entregadores; i++)
        pthread_create(&t_ent[i], NULL, thread_entregador, &args_ent[i]);

    /* Aguardar todas as threads operacionais */
    pthread_join(t_gerador, NULL);
    pthread_join(t_esteira, NULL);
    for (int i = 0; i < cfg.num_coletores; i++)
        pthread_join(t_col[i], NULL);
    for (int i = 0; i < cfg.num_entregadores; i++)
        pthread_join(t_ent[i], NULL);

    /* Encerrar thread de timeout (se a simulacao terminou antes dela) */
    simulacao_parar(&sim);   /* idempotente; garante que o timeout saia */
    pthread_join(t_timeout, NULL);

    /* Verificar resultados */
    int entregues = sim.stats.pacotes_entregues;
    int gerados   = sim.stats.pacotes_gerados;
    double duracao = agora_seg() - sim.stats.inicio;

    printf("Resultado:\n");
    printf("  Pacotes gerados   : %d / %d\n", gerados,   cfg.total_pacotes);
    printf("  Pacotes entregues : %d / %d\n", entregues, cfg.total_pacotes);
    printf("  Duracao           : %.2f s\n\n", duracao);

    simulacao_destroy(&sim);

    /* Assercoes finais */
    assert(!at.expirou && "simulacao deve terminar antes do timeout");
    assert(entregues == cfg.total_pacotes &&
           "todos os pacotes devem ter sido entregues");
    assert(gerados   == cfg.total_pacotes &&
           "todos os pacotes devem ter sido gerados");

    printf("[OK] Ciclo completo: %d pacotes gerados e entregues em %.2f s.\n",
           cfg.total_pacotes, duracao);
    return 0;
}
