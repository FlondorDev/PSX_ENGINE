
IF NOT DEFINED PSYQ_PATH goto define
goto compile

:define
    CALL C:\psyq\PSPATHS.BAT

:compile
    ccpsx -O2 -Xo0x80010000 C\main.c C\graph.c C\controller.c -o C.cpe
    cpe2x C.cpe
    del C.cpe