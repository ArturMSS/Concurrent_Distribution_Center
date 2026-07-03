#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "interface.h"
#include "simulacao.h"
#include "mapa.h"
#include "comum.h"

/* --- Layout (pixels) --- */
#define CELL_SIZE  20
#define PANEL_W   230
#define MARGIN     10
#define TITLE_H    28

/* --- Paleta de cores --- */
static const Color BG         = {18,  18,  24,  255};
static const Color C_LIVRE    = {32,  32,  40,  255};
static const Color C_PAREDE   = {160, 160, 170, 255};
static const Color C_ESTACAO  = {210, 170,   0, 255};
static const Color C_ESTEIRA  = {170,   0, 200, 255};
static const Color C_DESPACHO = {200, 100,   0, 255};
static const Color C_COLETOR  = {  0, 190,  80, 255};
static const Color C_ENTREG   = {  0, 200, 220, 255};
static const Color C_PANEL    = {26,  26,  38,  255};
static const Color C_TEXT     = {210, 210, 215, 255};
static const Color C_ACCENT   = {255, 210,  50, 255};
static const Color C_BAR_BG   = {50,  50,  60,  255};
static const Color C_BAR_FG   = {  0, 200,  80, 255};

void interface_init(void)
{
    /* Raylib precisa que InitWindow seja chamado a partir da mesma thread
     * que executara o loop de desenho (thread_interface). Nada a fazer aqui. */
    SetTraceLogLevel(LOG_WARNING);
}

void interface_fim(void)
{
    /* CloseWindow e invocado dentro de thread_interface. */
}

/* Retorna a cor de fundo e o simbolo de uma celula. */
static Color cor_celula(const Celula *c, int num_coletores, char *sym_out)
{
    if (c->ocupante != SEM_OCUPANTE) {
        if (c->ocupante <= num_coletores) { *sym_out = 'C'; return C_COLETOR; }
        else                              { *sym_out = 'E'; return C_ENTREG;  }
    }
    switch (c->tipo) {
        case CELULA_PAREDE:   *sym_out = '#'; return C_PAREDE;
        case CELULA_ESTACAO:  *sym_out = 'P'; return C_ESTACAO;
        case CELULA_ENTRADA:  *sym_out = 'I'; return C_ESTEIRA;
        case CELULA_SAIDA:    *sym_out = 'O'; return C_ESTEIRA;
        case CELULA_DESPACHO: *sym_out = 'D'; return C_DESPACHO;
        default:              *sym_out = '.'; return C_LIVRE;
    }
}

static void desenha_mapa(const Celula *copia, int W, int H, int num_coletores)
{
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            char sym = '.';
            Color bg = cor_celula(&copia[y * W + x], num_coletores, &sym);

            int px = MARGIN + x * CELL_SIZE;
            int py = TITLE_H + MARGIN + y * CELL_SIZE;

            DrawRectangle(px, py, CELL_SIZE - 1, CELL_SIZE - 1, bg);

            /* Simbolo centralizado (somente se a celula for grande o bastante) */
            if (CELL_SIZE >= 14 && sym != '.') {
                char buf[2] = {sym, '\0'};
                int fs = 10;
                int tw = MeasureText(buf, fs);
                Color fg = (bg.r + bg.g + bg.b > 380) ? BLACK : WHITE;
                DrawText(buf,
                         px + (CELL_SIZE - 1 - tw) / 2,
                         py + (CELL_SIZE - 1 - fs) / 2,
                         fs, fg);
            }
        }
    }
}

static void desenha_painel(int px0, int win_h,
                           int est_ocup, int est_tam,
                           int aguardando, int gerados,
                           int entregues, int meta,
                           double duracao, const char *nome)
{
    DrawRectangle(px0, 0, PANEL_W, win_h, C_PANEL);
    DrawLine(px0, 0, px0, win_h, (Color){60, 60, 80, 255});

    int x = px0 + MARGIN;
    int y = TITLE_H + MARGIN;
    int fs = 13;
    int gap = fs + 5;

    /* Titulo */
    DrawText("IDP Simulacao", x, y, 15, C_ACCENT);
    y += 20;
    DrawText(nome, x, y, fs - 1, C_TEXT);
    y += gap + 8;
    DrawLine(x, y, px0 + PANEL_W - MARGIN, y, (Color){60, 60, 80, 255});
    y += 6;

    /* Esteira */
    DrawText("Esteira", x, y, fs, C_ACCENT);
    y += gap;
    char buf[80];
    snprintf(buf, sizeof(buf), "  %d / %d posicoes", est_ocup, est_tam);
    DrawText(buf, x, y, fs, C_TEXT);
    y += gap + 6;

    /* Pacotes */
    DrawText("Pacotes", x, y, fs, C_ACCENT);
    y += gap;
    snprintf(buf, sizeof(buf), "  Aguardando : %d", aguardando);
    DrawText(buf, x, y, fs, C_TEXT);
    y += gap;
    snprintf(buf, sizeof(buf), "  Gerados    : %d", gerados);
    DrawText(buf, x, y, fs, C_TEXT);
    y += gap;
    snprintf(buf, sizeof(buf), "  Entregues  : %d / %d", entregues, meta);
    DrawText(buf, x, y, fs, C_TEXT);
    y += gap + 8;

    /* Barra de progresso */
    int bar_w = PANEL_W - 2 * MARGIN;
    float pct = (meta > 0) ? (float)entregues / (float)meta : 0.0f;
    if (pct > 1.0f) pct = 1.0f;
    DrawRectangle(x, y, bar_w, 12, C_BAR_BG);
    DrawRectangle(x, y, (int)(bar_w * pct), 12, C_BAR_FG);
    DrawRectangleLines(x, y, bar_w, 12, (Color){80, 80, 100, 255});
    y += 16;
    snprintf(buf, sizeof(buf), "%.0f%%  concluido", pct * 100.0f);
    DrawText(buf, x, y, fs - 1, C_TEXT);
    y += gap + 8;

    /* Tempo */
    DrawText("Tempo", x, y, fs, C_ACCENT);
    y += gap;
    snprintf(buf, sizeof(buf), "  %.1f s", duracao);
    DrawText(buf, x, y, fs, C_TEXT);
    y += gap + 12;

    DrawLine(x, y, px0 + PANEL_W - MARGIN, y, (Color){60, 60, 80, 255});
    y += 8;

    /* Legenda */
    DrawText("Legenda", x, y, fs, C_ACCENT);
    y += gap;

    struct { Color c; const char *label; } leg[] = {
        {C_ESTACAO,  "P  Estacao de coleta"},
        {C_ESTEIRA,  "I  Entrada esteira  "},
        {C_ESTEIRA,  "O  Saida esteira    "},
        {C_DESPACHO, "D  Despacho         "},
        {C_COLETOR,  "C  Robo coletor     "},
        {C_ENTREG,   "E  Robo entregador  "},
        {C_PAREDE,   "#  Parede           "},
    };
    for (int i = 0; i < 7; i++) {
        DrawRectangle(x, y + 2, 9, 9, leg[i].c);
        DrawText(leg[i].label, x + 13, y, fs - 1, C_TEXT);
        y += gap;
    }

    y += 8;
    DrawText("[Q] Encerrar simulacao", x, y, fs - 1, GRAY);
}

void *thread_interface(void *arg)
{
    Simulacao *sim = (Simulacao *)arg;
    int W = sim->mapa.largura;
    int H = sim->mapa.altura;

    int map_pw  = MARGIN + W * CELL_SIZE + MARGIN;
    int win_w   = map_pw + PANEL_W;
    int win_h   = TITLE_H + MARGIN + H * CELL_SIZE + MARGIN;
    if (win_h < 520) win_h = 520;

    SetTraceLogLevel(LOG_WARNING);
    InitWindow(win_w, win_h, "IDP - Centro de Distribuicao Automatizado");
    SetTargetFPS(15);

    Celula *copia = malloc((size_t)(W * H) * sizeof(Celula));
    if (!copia) {
        CloseWindow();
        return NULL;
    }

    while (!WindowShouldClose() && simulacao_ativa(sim)) {

        /* Captura de estado com mutexes */
        pthread_mutex_lock(&sim->mapa.mutex);
        for (int i = 0; i < W * H; i++)
            copia[i] = sim->mapa.celulas[i];
        pthread_mutex_unlock(&sim->mapa.mutex);

        pthread_mutex_lock(&sim->esteira.mutex);
        int est_ocup = sim->esteira.ocupadas;
        int est_tam  = sim->esteira.tamanho;
        pthread_mutex_unlock(&sim->esteira.mutex);

        pthread_mutex_lock(&sim->stats.mutex);
        int entregues = sim->stats.pacotes_entregues;
        int gerados   = sim->stats.pacotes_gerados;
        int meta      = sim->stats.total_pacotes;
        double inicio = sim->stats.inicio;
        pthread_mutex_unlock(&sim->stats.mutex);

        int aguardando = simulacao_pacotes_aguardando(sim);
        double duracao = agora_seg() - inicio;

        if (IsKeyPressed(KEY_Q))
            simulacao_parar(sim);

        BeginDrawing();
        ClearBackground(BG);

        /* Barra de titulo */
        DrawText("IDP - Centro de Distribuicao Automatizado",
                 MARGIN, (TITLE_H - 16) / 2, 16, C_ACCENT);
        DrawLine(0, TITLE_H - 1, win_w, TITLE_H - 1, (Color){50, 50, 70, 255});

        /* Mapa */
        desenha_mapa(copia, W, H, sim->cfg.num_coletores);

        /* Painel lateral */
        desenha_painel(map_pw, win_h,
                       est_ocup, est_tam, aguardando, gerados,
                       entregues, meta, duracao, sim->cfg.nome);

        EndDrawing();
    }

    simulacao_parar(sim);
    free(copia);
    CloseWindow();
    return NULL;
}
