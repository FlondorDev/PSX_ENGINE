#include "graph.h"
#include "cube.c"

DISPENV disp[2];
DRAWENV draw[2];
int db = 0;
u_long ot[2][OTLEN];    // Ordering table length
char pribuff[2][32768]; // Primitive buffer 32kb
char *nextpri;          // Next primitive pointer
TILE *tile;

void InitGraph()
{
    // Reset graphics
    ResetGraph(0);
    InitGeom();
    SetGeomOffset(CENTERX, CENTERY); // x, y offset
    SetGeomScreen(CENTERX);          // Distance between eye and screen

    // First buffer
    SetDefDispEnv(&disp[0], 0, 0, SCREENXRES, SCREENYRES);          // set Display env
    SetDefDrawEnv(&draw[0], 0, SCREENYRES, SCREENXRES, SCREENYRES); // set Draw env
    draw[0].isbg = 1;                                               // Enable clear
    setRGB0(&draw[0], 63, 0, 127);                                  // Set clear color (dark purple)

    // Second buffer
    SetDefDispEnv(&disp[1], 0, SCREENYRES, SCREENXRES, SCREENYRES);
    SetDefDrawEnv(&draw[1], 0, 0, SCREENXRES, SCREENYRES);
    draw[1].isbg = 1;
    setRGB0(&draw[1], 63, 0, 127);

    nextpri = pribuff[0]; // Set initial primitive pointer address

    SetDispMask(1); // Enable the display
}

void Blit()
{

    DrawSync(0); // Wait for any graphics processing to finish

    VSync(0); // Wait for vertical retrace

    PutDispEnv(&disp[db]); // Apply the DISPENV/DRAWENVs
    PutDrawEnv(&draw[db]);

    SetDispMask(1); // Enable the display

    DrawOTag(ot[db] + OTLEN - 1); // Draw the ordering table

    db = !db;              // Swap buffers on every pass (alternates between 1 and 0)
    nextpri = pribuff[db]; // Reset next primitive pointer
}

void CreateTileCOLOR(int x, int y, int w, int h, COLOR *c)
{
    tile = (TILE *)nextpri; // Cast next primitive

    setTile(tile);                   // Initialize the primitive (very important)
    setXY0(tile, x, y);              // Set primitive (x,y) position
    setWH(tile, w, h);               // Set primitive size
    setRGB0(tile, c->r, c->g, c->b); // Set color yellow
    addPrim(ot[db], tile);           // Add primitive to the ordering table

    nextpri += sizeof(TILE); // Advance the next primitive pointer
}

void CreateTileRGB(int x, int y, int w, int h, u_char r, u_char g, u_char b)
{
    tile = (TILE *)nextpri; // Cast next primitive

    setTile(tile);          // Initialize the primitive (very important)
    setXY0(tile, x, y);     // Set primitive (x,y) position
    setWH(tile, w, h);      // Set primitive size
    setRGB0(tile, r, g, b); // Set color yellow
    addPrim(ot[db], tile);  // Add primitive to the ordering table

    nextpri += sizeof(TILE); // Advance the next primitive pointer
}

void LoadDebugFont()
{
    // Load the internal font texture
    FntLoad(960, 0);
}

void PrintText(char *str, int x, int y)
{
    // Create the text stream
    FntOpen(x, y, 0, 0, 0, strlen(str));
    FntPrint(str);
    FntFlush(-1);
}

void CreateCube(u_long *ot, SVECTOR *Rotate, VECTOR *Trans, VECTOR *Scale, MATRIX *Matrix)
{
    long t, p, OTz, Flag; // t == vertex count, p == depth cueing interpolation value, OTz ==  value to create Z-ordered OT, Flag == see LibOver47.pdf, p.143
    POLY_G3 *poly = {0};
    u_int i;

    t = 0;

    RotMatrix(Rotate, Matrix);
    TransMatrix(Matrix, Trans);
    ScaleMatrix(Matrix, Scale);
    SetRotMatrix(Matrix);
    SetTransMatrix(Matrix);

    for (i = 0; i < (modelCube.len * 3); i += 3)
    {
        poly = (POLY_G3 *)nextpri;
        // Initialize the primitive and set its color values
        setPolyG3(poly);
        setRGB0(poly, modelCube.c[i].r, modelCube.c[i].g, modelCube.c[i].b);
        setRGB1(poly, modelCube.c[i + 2].r, modelCube.c[i + 2].g, modelCube.c[i + 2].b);
        setRGB2(poly, modelCube.c[i + 1].r, modelCube.c[i + 1].g, modelCube.c[i + 1].b);
        // Rotate, translate, and project the vectors and output the results into a primitive
        OTz = RotTransPers(&modelCube_mesh[modelCube_index[t]], (long *)&poly->x0, &p, &Flag);
        OTz += RotTransPers(&modelCube_mesh[modelCube_index[t + 2]], (long *)&poly->x1, &p, &Flag);
        OTz += RotTransPers(&modelCube_mesh[modelCube_index[t + 1]], (long *)&poly->x2, &p, &Flag);
        // Sort the primitive into the OT
        OTz /= 3;
        if ((OTz > 0) && (OTz < OTLEN))
            addPrim(&ot[OTz - 2], poly);

        nextpri += sizeof(POLY_G3);
        t += 3;
    }
}