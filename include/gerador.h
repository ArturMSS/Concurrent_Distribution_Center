#ifndef GERADOR_H
#define GERADOR_H

/*
 * gerador.h - Thread do Gerador de Pacotes.
 *
 * Recebe um (Simulacao *) como argumento. Gera pacotes ao longo do tempo e
 * os enfileira nas estacoes de coleta (recurso compartilhado).
 */

void *thread_gerador(void *arg);

#endif /* GERADOR_H */
