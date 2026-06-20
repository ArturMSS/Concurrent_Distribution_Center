#include "mapa.h"

int mapa_init(Mapa *m, int largura, int altura)
{
    /* TODO: alocar as celulas (largura*altura), marcar todas como livres e
     * sem ocupante, inicializar o mutex. Retornar 0 em sucesso. */
    (void)m;
    (void)largura;
    (void)altura;
    return 0;
}

void mapa_destroy(Mapa *m)
{
    /* TODO: liberar as celulas e destruir o mutex. */
    (void)m;
}

int mapa_valido(const Mapa *m, int x, int y)
{
    /* TODO: 1 se (x, y) esta dentro dos limites do mapa. */
    (void)m;
    (void)x;
    (void)y;
    return 0;
}

Celula *mapa_celula(Mapa *m, int x, int y)
{
    /* TODO: retornar ponteiro para a celula (x, y) ou NULL se invalida. */
    (void)m;
    (void)x;
    (void)y;
    return NULL;
}

int mapa_tenta_ocupar(Mapa *m, int x, int y, int id)
{
    /* TODO (secao critica): sob o mutex, ocupar (x, y) com 'id' se estiver
     * livre e nao for parede. Retornar 1 em sucesso, 0 caso contrario. */
    (void)m;
    (void)x;
    (void)y;
    (void)id;
    return 0;
}

void mapa_liberar(Mapa *m, int x, int y)
{
    /* TODO (secao critica): sob o mutex, marcar (x, y) como sem ocupante. */
    (void)m;
    (void)x;
    (void)y;
}

int mapa_tenta_mover(Mapa *m, Vec2 de, Vec2 para, int id)
{
    /* TODO (secao critica): sob o mutex, se 'para' estiver livre e nao for
     * parede, liberar 'de' e ocupar 'para'. Garantir que nenhuma celula
     * fique com dois ocupantes. Retornar 1 se moveu, 0 caso contrario. */
    (void)m;
    (void)de;
    (void)para;
    (void)id;
    return 0;
}
