#ifndef COMUM_H
#define COMUM_H

/*
 * comum.h - Tipos e utilidades compartilhadas por todo o projeto.
 */

/* Coordenada inteira (coluna x, linha y) usada no mapa. */
typedef struct {
    int x;
    int y;
} Vec2;

/* Retorna o tempo monotonico atual em segundos (para medir duracao). */
double agora_seg(void);

/* Dorme por 'ms' milissegundos (usado para dar ritmo as threads). */
void dormir_ms(int ms);

/* Distancia de Manhattan entre dois pontos do mapa. */
int distancia_manhattan(Vec2 a, Vec2 b);

#endif /* COMUM_H */
