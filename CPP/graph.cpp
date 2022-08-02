#include "graph.h"

void DisplayBuffer::InitBuffer(bool isDB, bool isSecondDB){
    if(isDB){
        if(!isSecondDB){
            SetDefDispEnv(&disp, 0, 0, 320, 240);   //set Display env
            SetDefDrawEnv(&draw, 0, 240, 320, 240); //set Draw env
        }else{
            SetDefDispEnv(&disp, 0, 240, 320, 240);   //set Display env
            SetDefDrawEnv(&draw, 0, 0, 320, 240); //set Draw env    
        }
    }else{
        ResetGraph(0);
        SetDefDispEnv(&disp, 0, 0, 320, 240);   //set Display env
        SetDefDrawEnv(&draw, 0, 0, 320, 240); //set Draw env
    }
    draw.isbg = 1;               // Enable clear
    setRGB0(&draw, 63, 0, 127);  // Set clear color (dark purple)
    ClearOTagR(ot, OTLEN);  // Clear ordering table
    nextpri = pribuff;           // Set initial primitive pointer address
}

void DisplayBuffer::Render() {
    
    DrawSync(0);                // Wait for any graphics processing to finish
    
    VSync(0);                   // Wait for vertical retrace

    PutDispEnv(&disp);      // Apply the DISPENV/DRAWENVs
    PutDrawEnv(&draw);

    SetDispMask(1);             // Enable the display

    DrawOTag(ot+OTLEN-1);   // Draw the ordering table
    
    nextpri = pribuff;      // Reset next primitive pointer
    ClearOTagR(ot, OTLEN);  // Clear ordering table
    
}

void DoubleDB::InitGraph(){
    // Reset graphics
    ResetGraph(0);
    DB[0].InitBuffer(true);
    DB[1].InitBuffer(true,true);
}

int DoubleDB::db_index = 0;

DisplayBuffer DoubleDB::DB[2];

void DoubleDB::Blit() {
    DB[db_index].Render();
    db_index = !db_index;                   // Swap buffers on every pass (alternates between 1 and 0)
}

void CreateTile(int x, int y, int w, int h, u_char r, u_char g, u_char b, DisplayBuffer* db){
    char* &nextpri = db->nextpri;

    setTile((TILE*)nextpri);              // Initialize the primitive (very important)
    setXY0((TILE*)nextpri, x, y);       // Set primitive (x,y) position
    setWH((TILE*)nextpri, w, h);        // Set primitive size
    setRGB0((TILE*)nextpri, r, g, b); // Set color yellow
    addPrim(db->ot, (TILE*)nextpri);      // Add primitive to the ordering table
    
    nextpri += sizeof(TILE);    // Advance the next primitive pointer
}

