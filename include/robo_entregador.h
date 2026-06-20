#ifndef ROBO_ENTREGADOR_H
#define ROBO_ENTREGADOR_H

/*
 * robo_entregador.h - Thread de um Robo Entregador.
 *
 * Recebe um (ArgRobo *). Cada robo entregador: vai ate a saida da esteira,
 * retira um pacote, leva ate um ponto de despacho (D) e o entrega,
 * atualizando o contador global de pacotes despachados.
 */

void *thread_entregador(void *arg);

#endif /* ROBO_ENTREGADOR_H */
