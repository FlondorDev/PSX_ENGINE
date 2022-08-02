#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "graph.h"
#include "controller.h"


int main() {
    int x = 0;
    int y = 0;
    int speed = 3;
    COLOR *col = new COLOR;
    col->r = 240;
    col->b = 80;
    col->g = 0;

    DoubleDB::InitGraph();
    InitController();
    
    while(1) {

        //INPUT - UPDATE
        GetInput(&x,&y,speed);

        //DRAW
        CreateTile(20, 30, 80, 60, col);
        CreateTile(x, y, 50, 50, 255, 255, 255);
        CreateTile(80, 150, 50, 50, 0, 255, 60);
        CreateTile(250, 180, 50, 50, 40, 10, 250);
        CreateTile(220, 80, 70, 50, 40, 70, 60);
        CreateTile(180, 120, 50, 70, 40, 100, 0);

        // Update the display
        DoubleDB::Blit();
    }
    
    delete col;
    return 0;
}
