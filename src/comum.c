#define _POSIX_C_SOURCE 199309L
#include "comum.h"
#include <time.h>
#include <stdlib.h>

double agora_seg(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + ts.tv_nsec * 1e-9;
}

void dormir_ms(int ms)
{
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

int distancia_manhattan(Vec2 a, Vec2 b)
{
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);
}
