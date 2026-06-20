#include "simulacao.h"
#include <string.h>

int simulacao_init(Simulacao *sim, Config cfg)
{
    /* TODO: a partir de 'cfg', inicializar mapa e esteira, alocar e montar
     * estacoes, despachos e robos, posicionar tudo no mapa, zerar as
     * estatisticas e marcar a simulacao como ativa. Retornar 0 em sucesso. */
    (void)sim;
    (void)cfg;
    return 0;
}

void simulacao_destroy(Simulacao *sim)
{
    /* TODO: liberar todos os recursos (mapa, esteira, estacoes, vetores) e
     * destruir os mutexes. */
    (void)sim;
}

int simulacao_ativa(Simulacao *sim)
{
    /* TODO: retornar 1 enquanto a simulacao deve continuar rodando. */
    (void)sim;
    return 0;
}

void simulacao_parar(Simulacao *sim)
{
    /* TODO: sinalizar o encerramento (faz todas as threads sairem do laco). */
    (void)sim;
}

void stats_inc_gerados(Simulacao *sim)
{
    /* TODO (secao critica): incrementar o contador de pacotes gerados. */
    (void)sim;
}

void stats_inc_entregues(Simulacao *sim)
{
    /* TODO (secao critica): incrementar o contador de pacotes entregues e,
     * se a meta foi atingida, encerrar a simulacao. */
    (void)sim;
}

int stats_pacotes_entregues(Simulacao *sim)
{
    /* TODO: retornar o numero de pacotes entregues. */
    (void)sim;
    return 0;
}

int simulacao_pacotes_aguardando(Simulacao *sim)
{
    /* TODO: somar os pacotes aguardando coleta em todas as estacoes. */
    (void)sim;
    return 0;
}
