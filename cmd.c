//
// Created by light on 18.04.2020.
//

#include <stdlib.h>
#include "pdp.h"

word get_warg3(word w) { // get an 3-bit argument
    int r = (w & 7); // command register
    int mode = ((w >> 3) & 7); // mode
    int arg = 0; // argument
    //trace(TRACE, "word %06o, mode %d, register %d\n", w, mode, r);
    switch(mode) {
        case 0:
            return reg[r];
        case 1:
            return w_read(reg[r]);
        case 2:
            arg = w_read(reg[r]);
            pc += WORD;
            return arg;
        case 3:
            arg = w_read(w_read(reg[r]));
            pc += WORD;
            return arg;
        case 4:
            pc -= WORD;
            return w_read(reg[r]);
        case 5:
            pc -= WORD;
            return w_read(w_read(reg[r]));
        case 6:
            return w_read(reg[r] + w_read(pc) + WORD);
            pc += WORD;
        case 7:
            return w_read(w_read(reg[r] + w_read(pc) + WORD));
            pc += WORD;
        default:
            trace(TRACE, "Address mode is higher than 7! Exiting...");
            exit(17);
    }
}

void do_halt(w) {
    trace(ERROR, "--- HALTED ---\n");
    trace(ERROR, "--- R0: %06o R1: %06o R2: %06o R3: %06o R4: %06o R5: %06o SP: %06o PC: %06o ---\n", reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]);
    exit(0);
}
void do_add(w) {
    word dd = get_warg3(w);
    word ss = get_warg3(w >> 6);
    //trace(TRACE, "mov res is %d, ss is %d, dd is %d\n", ss + dd, ss, dd);
    reg[(w & 7)] = dd + ss;
}
void do_mov(w) {
    word ss = get_warg3(w >> 6);
    reg[(w & 7)] = ss;
}
void do_nothing(w) {
}

Command cmd[] = {
        {0170000, 0010000, "mov", do_mov},
        {0170000, 0060000, "add", do_add},
        {0177777, 0000000, "halt", do_halt},
        {0177777, 0000001, "nothing", do_nothing}, //  резервная команда
        {0000000, 0000000, "unknown", do_nothing} // команда конца списка, есть пока не доделан код команд
};
