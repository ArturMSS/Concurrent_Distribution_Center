#include "gerador.h"
#include "simulacao.h"

void *thread_gerador(void *arg)
{
    /* TODO: laco do gerador. Enquanto a simulacao estiver ativa e a meta de
     * pacotes nao for atingida, criar pacotes e enfileira-los em estacoes de
     * coleta (recurso compartilhado), respeitando um intervalo de geracao.
     * (arg e um Simulacao *.) */
    (void)arg;
    return NULL;
}
