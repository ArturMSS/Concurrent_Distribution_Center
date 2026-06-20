#include "config.h"
#include "simulacao.h"
#include "gerador.h"
#include "esteira.h"
#include "robo_coletor.h"
#include "robo_entregador.h"
#include "interface.h"

/*
 * main.c - Ponto de entrada.
 *
 * Uso:  ./idp_sim [numero_do_cenario]
 *
 * Roteiro a implementar:
 *   1. Ler o cenario do argumento (config_carregar_cenario).
 *   2. simulacao_init(&sim, cfg).
 *   3. interface_init().
 *   4. Criar uma thread para cada elemento concorrente:
 *        - 1 gerador            (thread_gerador,      arg: &sim)
 *        - 1 esteira            (thread_esteira,      arg: &sim)
 *        - N robos coletores    (thread_coletor,      arg: ArgRobo*)
 *        - M robos entregadores (thread_entregador,   arg: ArgRobo*)
 *        - 1 interface          (thread_interface,    arg: &sim)
 *   5. pthread_join em todas as threads.
 *   6. interface_fim(); imprimir resumo; simulacao_destroy(&sim).
 */
int main(int argc, char **argv)
{
    /* TODO: implementar o roteiro acima. */
    (void)argc;
    (void)argv;
    return 0;
}
