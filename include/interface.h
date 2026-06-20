#ifndef INTERFACE_H
#define INTERFACE_H

/*
 * interface.h - Interface de acompanhamento em tempo real (ncurses).
 *
 * A interface acessa o estado compartilhado de forma sincronizada apenas
 * para LER e desenhar. Tambem captura o teclado (ex.: 'q' encerra).
 *
 * Deve exibir, no minimo: area de coleta, area de expedicao, posicao dos
 * robos coletores e entregadores, estado da esteira, quantidade de pacotes
 * aguardando coleta / na esteira / entregues, e o tempo de execucao.
 */

/* Inicializa o modo ncurses (chamar na thread principal antes das demais). */
void interface_init(void);

/* Encerra o modo ncurses e restaura o terminal. */
void interface_fim(void);

/* Thread da interface. Recebe um (Simulacao *). */
void *thread_interface(void *arg);

#endif /* INTERFACE_H */
