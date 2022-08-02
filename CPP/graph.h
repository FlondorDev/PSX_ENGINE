#ifndef _GRAPH_H_
#define _GRAPH_H_

    #include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
    #include <libetc.h>	// Includes some functions that controls the display
    #include <libgte.h>	// GTE header, not really used but libgpu.h depends on it
    #include <libgpu.h>	// GPU library header

    #define OTLEN 8         // Ordering table length (recommended to set as a define
                            // so it can be changed easily)

    typedef struct{
        u_char r,g,b;
    } COLOR;

    class DisplayBuffer{
        public:
            DISPENV disp;
            DRAWENV draw;
            u_long ot[OTLEN]; 
            char pribuff[32768];
            char *nextpri;          // Next primitive pointer
            void InitBuffer(bool isDB = false, bool isSecondDB = false);
            void Render();
    };

    class DoubleDB{
        private:
            static DisplayBuffer DB[2];
            static int db_index;
            DoubleDB(){}
        public:
            inline static DisplayBuffer* GetCurrentDB();
            static void InitGraph();
            static void Blit();
    };

    void CreateTile(int x, int y, int w, int h, COLOR* c);
    void CreateTile(int x, int y, int w, int h, u_char r, u_char g, u_char b);

#endif