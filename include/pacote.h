#ifndef PACOTE_H
#define PACOTE_H

/*
 * pacote.h - Representa um pacote que percorre o sistema:
 * estacao de coleta -> robo coletor -> esteira -> robo entregador -> despacho.
 */

typedef struct {
    int id;              /* identificador unico do pacote */
    int estacao_origem;  /* indice da estacao onde foi gerado */
    double criado_em;    /* instante (s) em que foi gerado */
} Pacote;

/* Aloca e inicializa um novo pacote. Liberar com free() ao ser entregue. */
Pacote *pacote_criar(int id, int estacao_origem);

#endif /* PACOTE_H */
