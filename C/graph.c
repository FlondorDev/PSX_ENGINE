#include "graph.h"

DISPENV disp[2];
DRAWENV draw[2];
int db = 0;
u_long ot[2][OTLEN];    // Ordering table length
char pribuff[2][32768]; // Primitive buffer 32kb
char *nextpri;          // Next primitive pointer
TILE* tile;

void InitGraph(){
    // Reset graphics
    ResetGraph(0);

    // First buffer
    SetDefDispEnv(&disp[0], 0, 0, 320, 240);   //set Display env
    SetDefDrawEnv(&draw[0], 0, 240, 320, 240); //set Draw env
    // Second buffer
    SetDefDispEnv(&disp[1], 0, 240, 320, 240);
    SetDefDrawEnv(&draw[1], 0, 0, 320, 240);

    draw[0].isbg = 1;               // Enable clear
    setRGB0(&draw[0], 63, 0, 127);  // Set clear color (dark purple)
    draw[1].isbg = 1;
    setRGB0(&draw[1], 63, 0, 127);
    nextpri = pribuff[0];           // Set initial primitive pointer address
}

void Blit() {
    
    DrawSync(0);                // Wait for any graphics processing to finish
    
    VSync(0);                   // Wait for vertical retrace

    PutDispEnv(&disp[db]);      // Apply the DISPENV/DRAWENVs
    PutDrawEnv(&draw[db]);

    SetDispMask(1);             // Enable the display

    DrawOTag(ot[db]+OTLEN-1);   // Draw the ordering table
    
    db = !db;                   // Swap buffers on every pass (alternates between 1 and 0)
    nextpri = pribuff[db];      // Reset next primitive pointer
    
}

void CreateTileCOLOR(int x, int y, int w, int h, COLOR* c){
    tile = (TILE*)nextpri;      // Cast next primitive

    setTile(tile);              // Initialize the primitive (very important)
    setXY0(tile, x, y);       // Set primitive (x,y) position
    setWH(tile, w, h);        // Set primitive size
    setRGB0(tile, c->r, c->g, c->b); // Set color yellow
    addPrim(ot[db], tile);      // Add primitive to the ordering table
    
    nextpri += sizeof(TILE);    // Advance the next primitive pointer
}

void CreateTileRGB(int x, int y, int w, int h, u_char r, u_char g, u_char b){
    tile = (TILE*)nextpri;      // Cast next primitive

    setTile(tile);              // Initialize the primitive (very important)
    setXY0(tile, x, y);       // Set primitive (x,y) position
    setWH(tile, w, h);        // Set primitive size
    setRGB0(tile, r, g, b); // Set color yellow
    addPrim(ot[db], tile);      // Add primitive to the ordering table
    
    nextpri += sizeof(TILE);    // Advance the next primitive pointer
}
