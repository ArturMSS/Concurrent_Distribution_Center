#include "robo_coletor.h"
#include "simulacao.h"

void *thread_coletor(void *arg)
{
    /* TODO: laco do robo coletor. Enquanto a simulacao estiver ativa:
     *   1. localizar uma estacao com pacote;
     *   2. deslocar-se ate ela;
     *   3. coletar o pacote (desenfileirar);
     *   4. levar ate a entrada da esteira e inserir (aguardar se ocupada).
     * (arg e um ArgRobo *: contem o Simulacao * e o Robo * controlado.) */
    (void)arg;
    return NULL;
}
