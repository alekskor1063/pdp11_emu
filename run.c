//
// Created by light on 17.04.2020.
//
#include <stdio.h>
#include <stdlib.h>
#include "pdp.h"

void run() {
    pc = 01000;
    int i = 0;
    while(1) {
        word w = w_read(pc);
        trace(TRACE, "PC = %06o WORD = %06o : ", pc, w);
        pc += WORD;
        i = 0;
        do { // считывание команд до конца списка (команды unknown)
            trace(DEBUG, "%s, cmd mask is %06o, word is %06o, opc is %06o, w and mask is %06o\n", cmd[i].name, cmd[i].mask, w, cmd[i].opcode, (w & cmd[i].mask));
            if ((w & cmd[i].mask) == cmd[i].opcode) {
                trace(ERROR, "%s", cmd[i].name);
                trace(ERROR, "\n");
                cmd[i].do_func(w);
            }
            i++;
        } while ((w & cmd[i-1].mask) != cmd[i-1].opcode);
        trace(TRACE, "--- R0: %06o R1: %06o R2: %06o R3: %06o R4: %06o R5: %06o SP: %06o PC: %06o ---\n", reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]);
    }
}
