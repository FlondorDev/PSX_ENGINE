#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "graph.h"
#include "controller.h"


int main() {
    int x = 0;
    int y = 0;
    int speed = 80;
    COLOR *col = new COLOR;
    col->r = 240;
    col->b = 80;
    col->g = 0;

    SVECTOR Rotate={ 232, 232, 0, 0 };                   // Rotation coordinates
    VECTOR  Trans={ 0, 0, CENTERX * 3, 0 };     // Translation coordinates
                                            // Scaling coordinates
    VECTOR  Scale={ ONE, ONE/2, ONE, 0 };     // ONE == 4096
    MATRIX  Matrix={0};                     // Matrix data for the GTE

    DoubleDB::InitGraph();
    InitController();
    LoadDebugFont();
    
    while(1) {
        ClearOTagR(DoubleDB::GetCurrentDB()->ot, OTLEN);  // Clear ordering table
        //INPUT - UPDATE
        GetInput(&x,&y,speed);

        Rotate.vy = x; // Pan
        Rotate.vx = y; // Tilt

        //DRAW
        //CreateTile(30, 50, 100, 80, col);
        //CreateTriangle(x, y, 25, 25, 255, 255, 0, DoubleDB::GetCurrentDB());
        //CreateTile(90, 150, 60, 50, 0, 255, 60);
        CreateCube(DoubleDB::GetCurrentDB(), &Rotate, &Trans, &Scale, &Matrix);
        // CreateTile(250, 180, 50, 50, 40, 10, 250);
        // CreateTile(220, 80, 70, 50, 40, 70, 60);
        // CreateTile(180, 120, 50, 70, 40, 100, 0);
        PrintText("hello world",10,20);
        // Update the display
        DoubleDB::Blit();
    }
    
    delete col;
    return 0;
}
