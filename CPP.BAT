
IF NOT DEFINED PSYQ_PATH goto define
goto compile

:define
    CALL C:\psyq\PSPATHS.BAT

:compile
    ccpsx -O2 -Xo0x80010000 CPP\main.cpp CPP\graph.cpp CPP\controller.cpp -o CPP.cpe
    cpe2x CPP.cpe
    del CPP.cpe