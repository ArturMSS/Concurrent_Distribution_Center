#include <ncurses.h>
#include <stdlib.h>
#include "interface.h"
#include "simulacao.h"
#include "mapa.h"
#include "comum.h"

/* Pares de cores (definidos em interface_init) */
#define COR_COLETOR    1   /* verde  */
#define COR_ENTREGADOR 2   /* ciano  */
#define COR_ESTRUTURA  3   /* amarelo — estacao, despacho */
#define COR_PAREDE     4   /* branco escuro */
#define COR_ESTEIRA    5   /* magenta — entrada/saida */

void interface_init(void)
{
    initscr();
    if (has_colors()) {
        start_color();
        init_pair(COR_COLETOR,    COLOR_GREEN,   COLOR_BLACK);
        init_pair(COR_ENTREGADOR, COLOR_CYAN,    COLOR_BLACK);
        init_pair(COR_ESTRUTURA,  COLOR_YELLOW,  COLOR_BLACK);
        init_pair(COR_PAREDE,     COLOR_WHITE,   COLOR_BLACK);
        init_pair(COR_ESTEIRA,    COLOR_MAGENTA, COLOR_BLACK);
    }
    noecho();
    cbreak();
    curs_set(0);
    nodelay(stdscr, TRUE);   /* getch() nao bloqueante */
    keypad(stdscr, TRUE);
}

void interface_fim(void)
{
    endwin();
}

void *thread_interface(void *arg)
{
    Simulacao *sim = (Simulacao *)arg;
    int W = sim->mapa.largura;
    int H = sim->mapa.altura;
    int cores = has_colors();

    /* Buffer de copia do mapa: permite soltar o mutex antes de desenhar */
    Celula *copia = malloc((size_t)(W * H) * sizeof(Celula));
    if (!copia) return NULL;

    while (simulacao_ativa(sim)) {

        /* --- Copiar mapa sob mutex (leitura rapida) --- */
        pthread_mutex_lock(&sim->mapa.mutex);
        for (int i = 0; i < W * H; i++)
            copia[i] = sim->mapa.celulas[i];
        pthread_mutex_unlock(&sim->mapa.mutex);

        /* --- Esteira --- */
        pthread_mutex_lock(&sim->esteira.mutex);
        int est_ocup = sim->esteira.ocupadas;
        int est_tam  = sim->esteira.tamanho;
        pthread_mutex_unlock(&sim->esteira.mutex);

        /* --- Estatisticas --- */
        pthread_mutex_lock(&sim->stats.mutex);
        int entregues  = sim->stats.pacotes_entregues;
        int gerados    = sim->stats.pacotes_gerados;
        int meta       = sim->stats.total_pacotes;
        double inicio  = sim->stats.inicio;
        pthread_mutex_unlock(&sim->stats.mutex);

        int aguardando = simulacao_pacotes_aguardando(sim);
        double duracao = agora_seg() - inicio;

        /* --- Desenhar --- */
        erase();

        mvprintw(0, 0, "IDP Sim  cenario: %-20s  (q = sair)", sim->cfg.nome);

        /* Mapa: linha y+1 para deixar linha 0 para o titulo */
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                const Celula *c = &copia[y * W + x];
                int sym, par = 0;

                if (c->ocupante != SEM_OCUPANTE) {
                    if (c->ocupante <= sim->cfg.num_coletores) {
                        sym = 'C'; par = COR_COLETOR;
                    } else {
                        sym = 'E'; par = COR_ENTREGADOR;
                    }
                } else {
                    switch (c->tipo) {
                        case CELULA_LIVRE:    sym = '.';                      break;
                        case CELULA_PAREDE:   sym = '#'; par = COR_PAREDE;   break;
                        case CELULA_ESTACAO:  sym = 'P'; par = COR_ESTRUTURA; break;
                        case CELULA_ENTRADA:  sym = 'I'; par = COR_ESTEIRA;  break;
                        case CELULA_SAIDA:    sym = 'O'; par = COR_ESTEIRA;  break;
                        case CELULA_DESPACHO: sym = 'D'; par = COR_ESTRUTURA; break;
                        default:              sym = '?';                      break;
                    }
                }

                if (cores && par) attron(COLOR_PAIR(par));
                mvaddch(y + 1, x, (chtype)sym);
                if (cores && par) attroff(COLOR_PAIR(par));
            }
        }

        /* Painel abaixo do mapa */
        int py = H + 2;
        mvprintw(py,     0, "Esteira           : %d / %d posicoes", est_ocup, est_tam);
        mvprintw(py + 1, 0, "Aguardando coleta : %d", aguardando);
        mvprintw(py + 2, 0, "Pacotes gerados   : %d", gerados);
        mvprintw(py + 3, 0, "Pacotes entregues : %d / %d", entregues, meta);
        mvprintw(py + 4, 0, "Tempo de execucao : %.1f s", duracao);
        mvprintw(py + 6, 0,
            "P=estacao  I/O=esteira  D=despacho  "
            "C=coletor  E=entregador  #=parede  .=livre");

        refresh();

        /* Teclado: nao bloqueante (nodelay) */
        int tecla = getch();
        if (tecla == 'q' || tecla == 'Q')
            simulacao_parar(sim);

        dormir_ms(100);
    }

    free(copia);
    return NULL;
}
