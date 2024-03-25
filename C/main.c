#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "graph.h"
#include "controller.h"

int main()
{
    int x = 0;
    int y = 0;
    int speed = 3;
    SVECTOR Rotate = {232, 232, 0, 0};     // Rotation coordinates
    VECTOR Trans = {0, 0, CENTERX * 3, 0}; // Translation coordinates
                                           // Scaling coordinates
    VECTOR Scale = {ONE, ONE / 2, ONE, 0}; // ONE == 4096
    MATRIX Matrix = {0};                   // Matrix data for the GTE

    COLOR *col = malloc(sizeof(COLOR));
    col->r = 240;
    col->b = 80;
    col->g = 0;

    InitGraph();
    InitController();
    LoadDebugFont();

    while (1)
    {
        ClearOTagR(ot[db], OTLEN); // Clear ordering table

        // INPUT - UPDATE
        GetInput(&x, &y, speed);

        Rotate.vy = x; // Pan
        Rotate.vx = y; // Tilt

        // DRAW

        CreateCube(ot[db], &Rotate, &Trans, &Scale, &Matrix);

        CreateTileCOLOR(20, 30, 80, 60, col);
        CreateTileRGB(x, y, 50, 50, 255, 255, 255);
        CreateTileRGB(80, 150, 50, 50, 0, 255, 60);
        CreateTileRGB(250, 180, 50, 50, 40, 10, 250);
        CreateTileRGB(220, 80, 70, 50, 40, 70, 60);
        CreateTileRGB(180, 120, 50, 70, 40, 100, 0);

        PrintText("hello world", 10, 20);

        // Update the display
        Blit();
    }

    free(col);
    return 0;
}
