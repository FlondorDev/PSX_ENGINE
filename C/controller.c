#include "controller.h"

char padbuff[2][34];
PADTYPE *pad;

void InitController(){
    // Initialize the pads
    InitPAD(padbuff[0], 34, padbuff[1], 34 );
    padbuff[0][0] = padbuff[0][1] = 0xff; //clear data, prevent faulty data
    padbuff[1][0] = padbuff[1][1] = 0xff; 

    // Begin polling
    StartPAD();
    
    // To avoid VSync Timeout error, may not be defined in PsyQ
    ChangeClearPAD(1);
}

void GetInput(int* x, int* y, int speed){
    // Parse controller input
    pad = (PADTYPE*)padbuff[0];

    // Only parse inputs when a controller is connected
    if( pad->stat == 0 )
    {
        // Only parse when a digital pad, 
        // dual-analog and dual-shock is connected
        if( ( pad->type == 0x4 ) || 
            ( pad->type == 0x5 ) || 
            ( pad->type == 0x7 ) )
        {
            if( !(pad->btn&PAD_UP) )            // test UP
            {
                *y -= speed;
            }
            else if( !(pad->btn&PAD_DOWN) )       // test DOWN
            {
                *y += speed;
            }
            if( !(pad->btn&PAD_LEFT) )          // test LEFT
            {
                *x -= speed;
            }
            else if( !(pad->btn&PAD_RIGHT) )    // test RIGHT
            {
                *x += speed;
            }
        }
    }
}