#ifndef _GRAPH_H_
#define _GRAPH_H_

    #include <sys/types.h>
    #include <libgte.h>
    #include <libgpu.h>
    #include <libetc.h>
    #include <stdio.h>
    // Sample vector model

    #define SCREENXRES 320
    #define SCREENYRES 240
    #define CENTERX     SCREENXRES/2
    #define CENTERY     SCREENYRES/2
    #define OTLEN       2048        // Maximum number of OT entries
    #define PRIMBUFFLEN 32768       // Maximum number of POLY_GT3 primitives

    typedef struct{
        u_char r,g,b;
    } COLOR;

    class DisplayBuffer{
        public:
            DISPENV disp;
            DRAWENV draw;
            u_long ot[OTLEN]; 
            char pribuff[PRIMBUFFLEN];
            char *nextpri;          // Next primitive pointer
            void InitBuffer(bool isDoubleB = false, bool isSecondDB = false);
            void Render();
    };

    class DoubleDB{
        private:
            static DisplayBuffer DB[2];
            static int db_index;
            DoubleDB(){}
        public:
            inline static DisplayBuffer* GetCurrentDB() { return &DB[db_index];}
            static void InitGraph();
            static void Blit();
    };

    void CreateTriangle(int x, int y, int w, int h, u_char r, u_char g, u_char b, DisplayBuffer* db);

    void CreateTile(int x, int y, int w, int h, u_char r, u_char g, u_char b, DisplayBuffer* db);

    inline void CreateTile(int x, int y, int w, int h, COLOR* c){
        CreateTile(x, y, w, h, c->r, c->g, c->b, DoubleDB::GetCurrentDB());
    }

    inline void CreateTile(int x, int y, int w, int h, COLOR* c, DisplayBuffer* db){
        CreateTile(x, y, w, h, c->r, c->g, c->b, db);
    }

    inline void CreateTile(int x, int y, int w, int h, u_char r, u_char g, u_char b){
        CreateTile(x, y, w, h, r, g, b, DoubleDB::GetCurrentDB());
    }

    void LoadDebugFont();
    void PrintText(char* str, int x, int y);
    void CreateCube(DisplayBuffer* db, SVECTOR* Rotate, VECTOR*  Trans, VECTOR*  Scale, MATRIX*  Matrix);

#endif