#include "graph.h"
#include <strings.h>
#include "cube.c"

void DisplayBuffer::InitBuffer(bool isDoubleB, bool isSecondDB){
    if(isDoubleB){
        if(!isSecondDB){
            SetDefDispEnv(&disp, 0, 0, SCREENXRES, SCREENYRES);   //set Display env
            SetDefDrawEnv(&draw, 0, SCREENYRES, SCREENXRES, SCREENYRES); //set Draw env
        }else{
            SetDefDispEnv(&disp, 0, SCREENYRES, SCREENXRES, SCREENYRES);   //set Display env
            SetDefDrawEnv(&draw, 0, 0, SCREENXRES, SCREENYRES); //set Draw env    
        }
    }else{
        ResetGraph(0);
        SetDefDispEnv(&disp, 0, 0, SCREENXRES, SCREENYRES);   //set Display env
        SetDefDrawEnv(&draw, 0, 0, SCREENXRES, SCREENYRES); //set Draw env
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

    DrawOTag(ot+OTLEN-1);   // Draw the ordering table
    
    nextpri = pribuff;      // Reset next primitive pointer
}

void DoubleDB::InitGraph(){
    // Reset graphics
    ResetGraph(0);
    InitGeom();
    SetGeomOffset(CENTERX, CENTERY);        // x, y offset
    SetGeomScreen(CENTERX);                 // Distance between eye and screen  
    DB[0].InitBuffer(true);
    DB[1].InitBuffer(true,true);
    SetDispMask(1);             // Enable the display
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
    addPrim(&db->ot, (TILE*)nextpri);      // Add primitive to the ordering table
    
    nextpri += sizeof(TILE);   // Advance the next primitive pointer
}

void CreateTriangle(int x, int y, int half_w, int half_h, u_char r, u_char g, u_char b, DisplayBuffer* db){
    // char const& triangle_size = 2;
    // char const& half_size = 1;
    // CreateTile(x - half_size, y - half_size, triangle_size, triangle_size, 255, 0, 0, db); // center

    // CreateTile(x - half_size, y - half_size - half_h, triangle_size, triangle_size, 255, 0, 0, db); // top
    // CreateTile(x - half_size - half_w, y - half_size + half_h, triangle_size, triangle_size, 255, 0, 0, db); // bottom_left
    // CreateTile(x - half_size + half_w, y - half_size + half_h, triangle_size, triangle_size, 255, 0, 0, db); // bottom_right

    char* &nextpri = db->nextpri;

    setPolyF3((POLY_F3*)nextpri);              // Initialize the primitive (very important)
    setXY3((POLY_F3*)nextpri, x- half_w, y - half_h, x - half_w, y + half_h, x + half_w, y + half_h);       // Set primitive (x,y) position
    setRGB0((POLY_F3*)nextpri, r, g, b); // Set color yellow
    addPrim(db->ot, (POLY_F3*)nextpri);      // Add primitive to the ordering table
    
    nextpri += sizeof(POLY_F3);   // Advance the next primitive pointer
}

void LoadDebugFont(){
    // Load the internal font texture
    FntLoad(960, 0);
}

void PrintText(char* str, int x, int y){
     // Create the text stream
    FntOpen(x, y, 0, 0, 0, strlen(str));
    FntPrint(str);
    FntFlush(-1);
}

void CreateCube(DisplayBuffer* db, SVECTOR* Rotate, VECTOR*  Trans, VECTOR*  Scale, MATRIX*  Matrix){
    long t, p, OTz, Flag;                // t == vertex count, p == depth cueing interpolation value, OTz ==  value to create Z-ordered OT, Flag == see LibOver47.pdf, p.143

    RotMatrix(Rotate, Matrix);
    TransMatrix(Matrix, Trans);
    ScaleMatrix(Matrix, Scale);
    SetRotMatrix(Matrix);
    SetTransMatrix(Matrix);
    POLY_G3* poly = {0}; 
    t = 0;
    for (u_int i = 0; i < (modelCube.len*3); i += 3) {      
        poly = (POLY_G3 *)db->nextpri;
        // Initialize the primitive and set its color values
        setPolyG3(poly);
        setRGB0(poly, modelCube.c[i].r , modelCube.c[i].g   , modelCube.c[i].b);
        setRGB1(poly, modelCube.c[i+2].r, modelCube.c[i+2].g, modelCube.c[i+2].b);
        setRGB2(poly, modelCube.c[i+1].r, modelCube.c[i+1].g, modelCube.c[i+1].b);
        // Rotate, translate, and project the vectors and output the results into a primitive
        OTz  = RotTransPers(&modelCube_mesh[modelCube_index[t]]  , (long*)&poly->x0, &p, &Flag);
        OTz += RotTransPers(&modelCube_mesh[modelCube_index[t+2]], (long*)&poly->x1, &p, &Flag);
        OTz += RotTransPers(&modelCube_mesh[modelCube_index[t+1]], (long*)&poly->x2, &p, &Flag);
        // Sort the primitive into the OT
        OTz /= 3;
        if ((OTz > 0) && (OTz < OTLEN)) addPrim(&db->ot[OTz-2], poly);

        db->nextpri += sizeof(POLY_G3);
        t+=3;
    }
}


