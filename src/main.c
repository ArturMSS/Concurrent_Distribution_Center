#include "config.h"
#include "simulacao.h"
#include "gerador.h"
#include "esteira.h"
#include "robo_coletor.h"
#include "robo_entregador.h"
#include "interface.h"
#include "comum.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    /* 1. Cenario: argv[1] seleciona o cenario (1-3) ou 0 para config dinamica */
    int n = (argc >= 2) ? atoi(argv[1]) : 1;
    Config cfg;
    if (n == 0) {
        if (config_carregar_dinamico(&cfg) != 0) {
            fprintf(stderr, "Erro: configuracao invalida.\n");
            return 1;
        }
    } else {
        if (n < 1 || n > NUM_CENARIOS) n = 1;
        cfg = config_carregar_cenario(n);
    }

    /* 2. Inicializar estado global */
    Simulacao sim;
    if (simulacao_init(&sim, cfg) != 0) {
        fprintf(stderr, "Erro: simulacao_init falhou.\n");
        return 1;
    }

    /* 3. Interface ncurses (deve ser inicializada antes das threads) */
    interface_init();

    /* 4. ArgRobo individuais — devem existir durante toda a vida das threads */
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

    /* 5. Criar threads */
    pthread_t t_gerador, t_esteira, t_interface;
    pthread_t t_col[cfg.num_coletores];
    pthread_t t_ent[cfg.num_entregadores];

    pthread_create(&t_gerador,   NULL, thread_gerador,   &sim);
    pthread_create(&t_esteira,   NULL, thread_esteira,   &sim);
    pthread_create(&t_interface, NULL, thread_interface, &sim);

    for (int i = 0; i < cfg.num_coletores; i++)
        pthread_create(&t_col[i], NULL, thread_coletor,    &args_col[i]);
    for (int i = 0; i < cfg.num_entregadores; i++)
        pthread_create(&t_ent[i], NULL, thread_entregador, &args_ent[i]);

    /* 6. Aguardar todas as threads */
    pthread_join(t_gerador,   NULL);
    pthread_join(t_esteira,   NULL);
    pthread_join(t_interface, NULL);
    for (int i = 0; i < cfg.num_coletores; i++)
        pthread_join(t_col[i], NULL);
    for (int i = 0; i < cfg.num_entregadores; i++)
        pthread_join(t_ent[i], NULL);

    /* 7. Encerrar interface e imprimir resumo */
    interface_fim();

    double duracao = agora_seg() - sim.stats.inicio;
    printf("\n=== Resumo: %s ===\n", cfg.nome);
    printf("  Pacotes entregues : %d / %d\n",
           sim.stats.pacotes_entregues, cfg.total_pacotes);
    printf("  Pacotes gerados   : %d\n", sim.stats.pacotes_gerados);
    printf("  Duracao           : %.1f s\n", duracao);

    /* 8. Liberar recursos */
    simulacao_destroy(&sim);
    return 0;
}
