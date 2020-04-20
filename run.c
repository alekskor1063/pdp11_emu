//
// Created by light on 17.04.2020.
//
#include "pdp.h"
#include <stdlib.h>

void run() {
    pc = 01000;
    int i;
    int p = 0;
    trace(INFO, "--------------- running ---------------\n");
    char flags[] = "----";
    while(1) {
        word w = w_read(pc);
        trace(TRACE, "%06o %06o : ", pc, w);
        //trace(TRACE, "%06o: ", pc);
        pc += WORD;
        p++;
        //mem_dump(0177562, 8);
        if (p > 30) {
            //exit(9);
        }
        i = 0;
        do { // считывание команд до конца списка (команды unknown)
            //trace(DEBUG, "%s, cmd mask is %06o, word is %06o, opc is %06o, w and mask is %06o\n", cmd[i].name, cmd[i].mask, w, cmd[i].opcode, (w & cmd[i].mask));
            if ((w & cmd[i].mask) == cmd[i].opcode) {
                trace(TRACE, "%s", cmd[i].name);
                cmd[i].do_func(w);
            }
            i++;
        } while ((w & cmd[i-1].mask) != cmd[i-1].opcode);
        if (N == 0) {
            flags[0] = '-';
        } else {
            flags[0] = 'N';
        }
        if (Z == 0) {
            flags[1] = '-';
        } else {
            flags[1] = 'Z';
        }
        if (V == 0) {
            flags[2] = '-';
        } else {
            flags[2] = 'V';
        }
        if (C == 0) {
            flags[3] = '-';
        } else {
            flags[3] = 'C';
        }
        display();
        trace(DEBUG, "\n%s 0:%06o 1:%06o 2:%06o 3:%06o 4:%06o 5:%06o S:%06o P:%06o\n", flags, reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]);
    }
}
