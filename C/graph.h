#ifndef _GRAPH_H_
#define _GRAPH_H_

    #include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
    #include <libetc.h>	// Includes some functions that controls the display
    #include <libgte.h>	// GTE header, not really used but libgpu.h depends on it
    #include <libgpu.h>	// GPU library header

    #define OTLEN 8         // Ordering table length (recommended to set as a define
                            // so it can be changed easily)

    typedef struct{
        unsigned char r,g,b;
    } COLOR;


    extern DISPENV disp[2];
    extern DRAWENV draw[2];
    extern int db;
    extern u_long ot[2][OTLEN];    // Ordering table length
    extern char pribuff[2][32768]; // Primitive buffer 32kb
    extern char *nextpri;          // Next primitive pointer
    extern TILE* tile;

    void InitGraph();
    void Blit();
    void CreateTileCOLOR(int x, int y, int w, int h, COLOR* c);
    void CreateTileRGB(int x, int y, int w, int h, u_char r, u_char g, u_char b);

#endif