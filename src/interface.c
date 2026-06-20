#include "interface.h"
#include "simulacao.h"

/* TODO: incluir <ncurses.h> e implementar a interface.
 * Lembre-se de acessar o estado compartilhado de forma sincronizada (apenas
 * para leitura/desenho) e de capturar o teclado (ex.: 'q' encerra). */

void interface_init(void)
{
    /* TODO: inicializar o modo ncurses (initscr, cores, noecho, etc.). */
}

void interface_fim(void)
{
    /* TODO: encerrar o modo ncurses (endwin) e restaurar o terminal. */
}

void *thread_interface(void *arg)
{
    /* TODO: laco de desenho. Enquanto a simulacao estiver ativa, desenhar o
     * mapa, a esteira e o painel de informacoes (pacotes aguardando, na
     * esteira, entregues, tempo de execucao) e tratar o teclado.
     * (arg e um Simulacao *.) */
    (void)arg;
    return NULL;
}
