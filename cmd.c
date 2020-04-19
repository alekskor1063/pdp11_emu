//
// Created by light on 18.04.2020.
//

#include <stdlib.h>
#include "pdp.h"

word get_warg3(word w) { // get an 3-bit argument
    int r = (w & 7); // command register
    int mode = ((w >> 3) & 7); // mode
    int arg; // argument
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
            trace(ERROR, "Address mode is higher than 7! Exiting...");
            exit(17);
    }
}

void write_warg3(word num, word w) { // num to write and command
    int r = (w & 7); // command register
    int mode = ((w >> 3) & 7); // mode
    //trace(TRACE, "word %06o, mode %d, register %d\n", w, mode, r);
    switch(mode) {
        case 0:
            reg[r] = num;
            break;
        case 1:
            w_write(reg[r], num);
            break;
        case 2:
            w_write(reg[r], num);
            break;
            pc += WORD;
        case 3:
            w_write(w_read(num), reg[r]);
            pc += WORD;
            break;
        case 4:
            pc -= WORD;
            //trace(ERROR, "pc is %06o\n", pc);
            w_write(reg[r], num);
            break;
        case 5:
            pc -= WORD;
            w_write(w_read(num), reg[r]);
            break;
            /*
        case 6: // not works
            w_write(reg[r] + w_write(pc) + WORD);
            pc += WORD;
        case 7: // not works
            w_write(w_write(reg[r] + w_write(pc) + WORD));
            pc += WORD;
             */
        default:
            trace(ERROR, "Address mode is higher than 7! Exiting..");
            exit(17);
    }
}

void do_halt(word w) {
    trace(TRACE, "\n");
    trace(DEBUG, "---- 0:%06o 1:%06o 2:%06o 3:%06o 4:%06o 5:%06o S:%06o P:%06o\n", reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]);
    trace(INFO, "\n---------------- halted ---------------\n");
    trace(INFO, "r0=%06o r2=%06o r4=%06o sp=%06o\nr1=%06o r3=%06o r5=%06o pc=%06o", reg[0],  reg[2], reg[4], reg[6], reg[1], reg[3], reg[5], reg[7]);
    exit(0);
}
void do_add(word w) {
    word dd = get_warg3(w);
    word ss = get_warg3(w >> 6);
    trace(TRACE, "      r%d,r%d                  R%d=%06o, R%d=%06o\n", ((w >> 6) & 7), (w & 7), ((w >> 6) & 7) , ss, (w & 7), dd);
    write_warg3(ss + dd, w);
}
void do_mov(word w) {
    word ss = get_warg3(w >> 6);
    if (((w >> 6) & 7) == 7) {
        trace(TRACE, "      #%06o,r%d             [%06o]=%06o\n", ss, (w & 7), reg[((w >> 6) & 7)] - WORD, ss);
    } else {
        trace(TRACE, "      r %d,r%d                  R %d=%06o\n", ((w >> 6) & 7), (w & 7), ((w >> 6) & 7), ss);
    }
    write_warg3(ss, w);
}
void do_nothing(word w) {
    exit(020);
}

Command cmd[] = {
        {0170000, 0010000, "mov", do_mov},
        {0170000, 0060000, "add", do_add},
        {0177777, 0000000, "halt", do_halt},
        {0177777, 0000001, "nothing", do_nothing}, //  резервная команда
        {0000000, 0000000, "unknown", do_nothing} // команда конца списка, есть пока не доделан код команд
};
