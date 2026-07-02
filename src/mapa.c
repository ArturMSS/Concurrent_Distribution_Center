#include "mapa.h"
#include <stdlib.h>
#include <string.h>

int mapa_init(Mapa *m, int largura, int altura)
{
    m->largura = largura;
    m->altura  = altura;
    m->celulas = calloc((size_t)(largura * altura), sizeof(Celula));
    if (!m->celulas) return -1;

    for (int i = 0; i < largura * altura; i++) {
        m->celulas[i].tipo     = CELULA_LIVRE;
        m->celulas[i].ocupante = SEM_OCUPANTE;
    }

    if (pthread_mutex_init(&m->mutex, NULL) != 0) {
        free(m->celulas);
        m->celulas = NULL;
        return -1;
    }
    return 0;
}

void mapa_destroy(Mapa *m)
{
    free(m->celulas);
    m->celulas = NULL;
    pthread_mutex_destroy(&m->mutex);
}

int mapa_valido(const Mapa *m, int x, int y)
{
    return x >= 0 && x < m->largura && y >= 0 && y < m->altura;
}

Celula *mapa_celula(Mapa *m, int x, int y)
{
    if (!mapa_valido(m, x, y)) return NULL;
    return &m->celulas[y * m->largura + x];
}

/* --- secoes criticas --- */

int mapa_tenta_ocupar(Mapa *m, int x, int y, int id)
{
    if (!mapa_valido(m, x, y)) return 0;

    pthread_mutex_lock(&m->mutex);
    Celula *c = &m->celulas[y * m->largura + x];
    int ok = 0;
    if (c->tipo != CELULA_PAREDE && c->ocupante == SEM_OCUPANTE) {
        c->ocupante = id;
        ok = 1;
    }
    pthread_mutex_unlock(&m->mutex);
    return ok;
}

void mapa_liberar(Mapa *m, int x, int y)
{
    if (!mapa_valido(m, x, y)) return;

    pthread_mutex_lock(&m->mutex);
    m->celulas[y * m->largura + x].ocupante = SEM_OCUPANTE;
    pthread_mutex_unlock(&m->mutex);
}

int mapa_tenta_mover(Mapa *m, Vec2 de, Vec2 para, int id)
{
    if (!mapa_valido(m, para.x, para.y)) return 0;

    pthread_mutex_lock(&m->mutex);
    Celula *dest = &m->celulas[para.y * m->largura + para.x];
    int ok = 0;
    if (dest->tipo != CELULA_PAREDE && dest->ocupante == SEM_OCUPANTE) {
        if (mapa_valido(m, de.x, de.y))
            m->celulas[de.y * m->largura + de.x].ocupante = SEM_OCUPANTE;
        dest->ocupante = id;
        ok = 1;
    }
    pthread_mutex_unlock(&m->mutex);
    return ok;
}
