#include "robo_entregador.h"
#include "simulacao.h"

void *thread_entregador(void *arg)
{
    /* TODO: laco do robo entregador. Enquanto a simulacao estiver ativa:
     *   1. ir ate a saida da esteira;
     *   2. retirar um pacote (se houver);
     *   3. levar ate um ponto de despacho e entregar;
     *   4. atualizar o contador global de pacotes entregues.
     * (arg e um ArgRobo *: contem o Simulacao * e o Robo * controlado.) */
    (void)arg;
    return NULL;
}
