#ifndef CONFIG_H
#define CONFIG_H

/*
 * config.h - Parametros configuraveis da simulacao.
 *
 * O enunciado permite configuracao ESTATICA (cenarios pre-definidos) ou
 * DINAMICA (parametros lidos do usuario). Este esqueleto usa cenarios
 * estaticos selecionados por argumento de linha de comando; a leitura
 * dinamica fica como exercicio (ver config_carregar_dinamico).
 */

typedef struct {
    char nome[64];          /* nome descritivo do cenario */

    int largura;            /* dimensoes do mapa (em celulas) */
    int altura;

    int num_coletores;      /* quantidade de robos coletores */
    int num_entregadores;   /* quantidade de robos entregadores */
    int num_estacoes;       /* quantidade de estacoes geradoras (P) */
    int num_despachos;      /* quantidade de pontos de despacho (D) */
    int num_obstaculos;     /* quantidade de paredes/obstaculos internos */

    int tamanho_esteira;    /* numero de posicoes da esteira */

    int total_pacotes;      /* condicao de termino: pacotes a entregar */

    int intervalo_geracao_ms; /* intervalo base entre geracoes de pacote */
    int passo_ms;             /* duracao de um "tick"/passo da simulacao */

    unsigned int seed;      /* semente para geracao pseudoaleatoria */
} Config;

/* Numero de cenarios estaticos pre-definidos disponiveis. */
#define NUM_CENARIOS 3

/* Carrega um dos cenarios estaticos (n de 1 a NUM_CENARIOS). */
Config config_carregar_cenario(int n);

/* TODO (configuracao dinamica): preencher uma Config a partir de entrada
 * do usuario (stdin, argumentos ou arquivo). Retornar 0 em caso de sucesso. */
int config_carregar_dinamico(Config *cfg);

#endif /* CONFIG_H */
