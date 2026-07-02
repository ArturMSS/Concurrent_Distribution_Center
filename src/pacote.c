#include "pacote.h"
#include "comum.h"
#include <stdlib.h>

Pacote *pacote_criar(int id, int estacao_origem)
{
    Pacote *p = malloc(sizeof(Pacote));
    if (!p) return NULL;
    p->id             = id;
    p->estacao_origem = estacao_origem;
    p->criado_em      = agora_seg();
    return p;
}
