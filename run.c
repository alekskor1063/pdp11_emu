//
// Created by light on 17.04.2020.
//
#include "pdp.h"

void run() {
    pc = 01000;
    int i;
    trace(INFO, "--------------- running ---------------\n");
    while(1) {
        word w = w_read(pc);
        //trace(TRACE, "%06o %06o : ", pc, w);
        trace(TRACE, "%06o: ", pc);
        pc += WORD;
        i = 0;
        do { // считывание команд до конца списка (команды unknown)
            //trace(DEBUG, "%s, cmd mask is %06o, word is %06o, opc is %06o, w and mask is %06o\n", cmd[i].name, cmd[i].mask, w, cmd[i].opcode, (w & cmd[i].mask));
            if ((w & cmd[i].mask) == cmd[i].opcode) {
                trace(TRACE, "%s", cmd[i].name);
                cmd[i].do_func(w);
            }
            i++;
        } while ((w & cmd[i-1].mask) != cmd[i-1].opcode);
        trace(DEBUG, "---- 0:%06o 1:%06o 2:%06o 3:%06o 4:%06o 5:%06o S:%06o P:%06o\n", reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]);
    }
}
