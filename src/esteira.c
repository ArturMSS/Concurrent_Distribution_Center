#include "esteira.h"

int esteira_init(Esteira *e, int tamanho)
{
    /* TODO: alocar o vetor de posicoes (todas vazias), zerar contadores,
     * inicializar o mutex. Retornar 0 em sucesso. */
    (void)e;
    (void)tamanho;
    return 0;
}

void esteira_destroy(Esteira *e)
{
    /* TODO: liberar o vetor (e os pacotes restantes) e destruir o mutex. */
    (void)e;
}

int esteira_tenta_inserir(Esteira *e, Pacote *p)
{
    /* TODO (secao critica): sob o mutex, inserir 'p' na entrada (posicoes[0])
     * se estiver livre. Retornar 1 se inseriu, 0 se ocupada. */
    (void)e;
    (void)p;
    return 0;
}

void esteira_avancar(Esteira *e)
{
    /* TODO (secao critica): sob o mutex, avancar cada pacote uma posicao em
     * direcao a saida, se a proxima estiver livre. */
    (void)e;
}

Pacote *esteira_retirar_saida(Esteira *e)
{
    /* TODO (secao critica): sob o mutex, retirar e retornar o pacote da saida
     * (ultima posicao), ou NULL se vazia. */
    (void)e;
    return NULL;
}

int esteira_ocupadas(Esteira *e)
{
    /* TODO: retornar quantos pacotes estao na esteira. */
    (void)e;
    return 0;
}

void *thread_esteira(void *arg)
{
    /* TODO: laco da esteira. Enquanto a simulacao estiver ativa, chamar
     * esteira_avancar a cada 'passo_ms'. (arg e um Simulacao *.) */
    (void)arg;
    return NULL;
}
