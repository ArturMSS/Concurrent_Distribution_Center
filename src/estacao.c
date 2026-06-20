#include "estacao.h"

int estacao_init(Estacao *est, Vec2 pos)
{
    /* TODO: inicializar a fila vazia na posicao 'pos' e o mutex. */
    (void)est;
    (void)pos;
    return 0;
}

void estacao_destroy(Estacao *est)
{
    /* TODO: liberar os nos da fila (e seus pacotes) e destruir o mutex. */
    (void)est;
}

void estacao_enfileirar(Estacao *est, Pacote *p)
{
    /* TODO (secao critica): sob o mutex, inserir 'p' no fim da fila. */
    (void)est;
    (void)p;
}

Pacote *estacao_desenfileirar(Estacao *est)
{
    /* TODO (secao critica): sob o mutex, remover e retornar o primeiro pacote
     * da fila, ou NULL se vazia. */
    (void)est;
    return NULL;
}

int estacao_quantidade(Estacao *est)
{
    /* TODO: retornar quantos pacotes aguardam coleta nesta estacao. */
    (void)est;
    return 0;
}
