#include "config.h"
#include <string.h>
#include <stdio.h>

Config config_carregar_cenario(int n)
{
    Config c;
    memset(&c, 0, sizeof(c));

    switch (n) {
        default:
        case 1:
            snprintf(c.nome, sizeof(c.nome), "Simples");
            c.largura              = 18;
            c.altura               = 12;
            c.num_coletores        = 2;
            c.num_entregadores     = 1;
            c.num_estacoes         = 3;
            c.num_despachos        = 2;
            c.num_obstaculos       = 4;
            c.tamanho_esteira      = 6;
            c.total_pacotes        = 20;
            c.intervalo_geracao_ms = 1200;
            c.passo_ms             = 300;
            c.seed                 = 42u;
            break;

        case 2:
            snprintf(c.nome, sizeof(c.nome), "Intermediario");
            c.largura              = 24;
            c.altura               = 16;
            c.num_coletores        = 3;
            c.num_entregadores     = 2;
            c.num_estacoes         = 4;
            c.num_despachos        = 3;
            c.num_obstaculos       = 12;
            c.tamanho_esteira      = 8;
            c.total_pacotes        = 40;
            c.intervalo_geracao_ms = 900;
            c.passo_ms             = 200;
            c.seed                 = 137u;
            break;

        case 3:
            snprintf(c.nome, sizeof(c.nome), "Alta concorrencia");
            c.largura              = 32;
            c.altura               = 20;
            c.num_coletores        = 5;
            c.num_entregadores     = 4;
            c.num_estacoes         = 6;
            c.num_despachos        = 4;
            c.num_obstaculos       = 24;
            c.tamanho_esteira      = 12;
            c.total_pacotes        = 80;
            c.intervalo_geracao_ms = 600;
            c.passo_ms             = 150;
            c.seed                 = 271u;
            break;
    }

    return c;
}

/* Le um inteiro da stdin. Se o usuario pressionar Enter sem digitar nada,
 * usa 'padrao'. Retorna o valor lido ou o padrao. */
static int ler_int(const char *prompt, int padrao, int minimo, int maximo)
{
    char buf[64];
    int v;
    for (;;) {
        printf("%s [%d]: ", prompt, padrao);
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin)) return padrao;
        if (buf[0] == '\n') return padrao;
        if (sscanf(buf, "%d", &v) == 1 && v >= minimo && v <= maximo)
            return v;
        printf("  Valor invalido. Informe um inteiro entre %d e %d.\n",
               minimo, maximo);
    }
}

int config_carregar_dinamico(Config *cfg)
{
    memset(cfg, 0, sizeof(*cfg));

    printf("\n=== Configuracao dinamica ===\n");
    printf("Pressione Enter para usar o valor entre colchetes.\n\n");

    snprintf(cfg->nome, sizeof(cfg->nome), "Personalizado");

    cfg->largura          = ler_int("Largura do mapa (colunas)(min 8, max 64)", 18,  8, 64);
    cfg->altura           = ler_int("Altura do mapa (linhas)(min 6, max 40)",   12,  6, 40);
    cfg->num_coletores    = ler_int("Robos coletores(min 1, max 10)",            2,  1, 10);
    cfg->num_entregadores = ler_int("Robos entregadores(min 1, max 10)",         1,  1, 10);
    cfg->num_estacoes     = ler_int("Estacoes de coleta (P)(min 1, max 10)",     3,  1, 10);
    cfg->num_despachos    = ler_int("Pontos de despacho (D)(min 1, max 10)",     2,  1, 10);
    cfg->num_obstaculos   = ler_int("Obstaculos (#)(min 0, max 40)",             4,  0, 40);
    cfg->tamanho_esteira  = ler_int("Tamanho da esteira(min 2, max 30)",         6,  2, 30);
    cfg->total_pacotes    = ler_int("Total de pacotes (meta)(min 1, max 500)",   20,  1, 500);
    cfg->intervalo_geracao_ms =
                            ler_int("Intervalo de geracao (ms)(min 50, max 10000)", 1200, 50, 10000);
    cfg->passo_ms         = ler_int("Duracao de um tick (ms)(min 50, max 2000)",   300, 50,  2000);
    cfg->seed             = (unsigned int)ler_int("Semente aleatoria(min 0, max 99999)", 42, 0, 99999);

    /* Sanidade minima: ao menos uma celula livre por robo e por estrutura */
    int total_celulas = cfg->largura * cfg->altura;
    int ocupados = cfg->num_coletores + cfg->num_entregadores
                 + cfg->num_estacoes  + cfg->num_despachos
                 + cfg->num_obstaculos + 2; /* +2: entrada e saida da esteira */
    if (ocupados >= total_celulas) {
        fprintf(stderr,
            "Erro: mapa muito pequeno para acomodar todos os elementos "
            "(%d celulas, %d necessarias).\n", total_celulas, ocupados + 1);
        return -1;
    }

    printf("\nCenario \"%s\" configurado com sucesso.\n\n", cfg->nome);
    return 0;
}
