#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>
#include <stdio.h>

#define SCREENXRES 320
#define SCREENYRES 240
#define CENTERX SCREENXRES / 2
#define CENTERY SCREENYRES / 2
#define OTLEN 2048        // Maximum number of OT entries
#define PRIMBUFFLEN 32768 // Maximum number of POLY_GT3 primitives

typedef struct
{
    u_char r, g, b;
} COLOR;

extern DISPENV disp[2];
extern DRAWENV draw[2];
extern int db;
extern u_long ot[2][OTLEN];          // Ordering table length
extern char pribuff[2][PRIMBUFFLEN]; // Primitive buffer 32kb
extern char *nextpri;                // Next primitive pointer
extern TILE *tile;

void InitGraph();
void Blit();
void CreateTileCOLOR(int x, int y, int w, int h, COLOR *c);
void CreateTileRGB(int x, int y, int w, int h, u_char r, u_char g, u_char b);

void LoadDebugFont();
void PrintText(char *str, int x, int y);
void CreateCube(u_long *ot, SVECTOR *Rotate, VECTOR *Trans, VECTOR *Scale, MATRIX *Matrix);

#endif