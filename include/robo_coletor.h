#ifndef ROBO_COLETOR_H
#define ROBO_COLETOR_H

/*
 * robo_coletor.h - Thread de um Robo Coletor.
 *
 * Recebe um (ArgRobo *). Cada robo coletor: localiza uma estacao com pacote,
 * desloca-se ate ela, coleta o pacote, leva ate a entrada da esteira e o
 * insere. Se a entrada estiver ocupada, aguarda.
 */

void *thread_coletor(void *arg);

#endif /* ROBO_COLETOR_H */
