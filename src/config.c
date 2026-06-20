#include "config.h"
#include <string.h>

Config config_carregar_cenario(int n)
{
    Config c;
    memset(&c, 0, sizeof(c));

    /* TODO: preencher os campos de 'c' de acordo com o cenario 'n'
     * (1 a NUM_CENARIOS). Recomenda-se ao menos 3 cenarios distintos:
     *   c.largura, c.altura, c.num_coletores, c.num_entregadores,
     *   c.num_estacoes, c.num_despachos, c.num_obstaculos,
     *   c.tamanho_esteira, c.total_pacotes, c.intervalo_geracao_ms,
     *   c.passo_ms, c.seed, snprintf(c.nome, ...). */
    (void)n;
    return c;
}

int config_carregar_dinamico(Config *cfg)
{
    /* TODO (opcional): ler os parametros do usuario em tempo de execucao. */
    (void)cfg;
    return 0;
}
