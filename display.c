//
// Created by light on 20.04.2020.
//
#include "pdp.h"

void display () {
    //mem[0177564] = 0000000;
    if (w_read(0177566) != 0) {
        char symb[1];
        symb[0] = mem[0177566];
        trace(TRACE, "\n");
        trace(ERROR, "%s", symb);
        mem[0177566] = 0;
    }
    mem[0177564] = 0000200;
}