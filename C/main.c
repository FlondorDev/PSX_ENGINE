#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "graph.h"
#include "controller.h"


int main() {
    int x = 0;
    int y = 0;
    int speed = 3;
    COLOR *col = malloc(sizeof(COLOR));
    col->r = 240;
    col->b = 80;
    col->g = 0;

    InitGraph();
    InitController();
    
    while(1) {
        ClearOTagR(ot[db], OTLEN);  // Clear ordering table

        //INPUT - UPDATE
        GetInput(&x,&y,speed);

        //DRAW
        CreateTileCOLOR(20, 30, 80, 60, col);
        CreateTileRGB(x, y, 50, 50, 255, 255, 255);
        CreateTileRGB(80, 150, 50, 50, 0, 255, 60);
        CreateTileRGB(250, 180, 50, 50, 40, 10, 250);
        CreateTileRGB(220, 80, 70, 50, 40, 70, 60);
        CreateTileRGB(180, 120, 50, 70, 40, 100, 0);

        // Update the display
        Blit();
    }
    
    free(col);
    return 0;
}
