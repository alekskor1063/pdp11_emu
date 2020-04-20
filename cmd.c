//
// Created by light on 18.04.2020.
//

#include <stdlib.h>
#include "pdp.h"

void change (int flag) {
    if (flag == 0) {
        flag = 1;
    } else {
        flag = 0;
    }
}

word get_warg3(word w) { // get an 3-bit argument
    int r = (w & 7); // command register
    int mode = ((w >> 3) & 7); // mode
    word arg; // argument
    //trace(TRACE, "word %06o, mode %d, register %d\n", w, mode, r);
    switch(mode) {
        case 0:
            return reg[r];
        case 1:
            return w_read(reg[r]);
        case 2:
            arg = w_read(reg[r]);
            reg[r] += WORD;
            return arg;
        case 3:
            arg = w_read(w_read(reg[r]));
            reg[r] += WORD;
            return arg;
        case 4:
            reg[r] -= WORD;
            return w_read(reg[r]);
        case 5:
            reg[r] -= WORD;
            return w_read(w_read(reg[r]));
        case 6:
            pc += WORD;
            return w_read(reg[r] + w_read(pc - WORD));
        case 7:
            pc += WORD;
            return w_read(w_read(reg[r] + w_read(pc - WORD)));
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
            pc += WORD;
            break;
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
        case 6:
            w_write((reg[r] + w_read(pc) + WORD), num);
            pc += WORD;
            break;
        case 7:
            w_write(w_read(reg[r] + w_read(pc) + WORD), num);
            pc += WORD;
            break;
        default:
            trace(ERROR, "Address mode is higher than 7! Exiting..");
            exit(16);
    }
}


byte get_barg3(word w) {
    int r = (w & 7); // command register
    int mode = ((w >> 3) & 7); // mode
    byte arg; // argument
    //trace(TRACE, "word %06o, mode %d, register %d\n", w, mode, r);
    switch(mode) {
        case 0:
            return reg[r];
        case 1:
            return b_read(reg[r]);
        case 2:
            arg = b_read(reg[r]);
            if (r == 6 || r == 7) {
                reg[r] += WORD;
            } else {
                reg[r] += 1;
            }
            return arg;
        case 3:
            arg = b_read(w_read(reg[r]));
            if (r == 6 || r == 7) {
                reg[r] += WORD;
            } else {
                reg[r] += 1;
            }
            return arg;
        case 4:
            if (r == 6 || r == 7) {
                reg[r] -= WORD;
            } else {
                reg[r] -= 1;
            }
            return b_read(reg[r]);
        case 5:
            if (r == 6 || r == 7) {
                reg[r] -= WORD;
            } else {
                reg[r] -= 1;
            }
            return b_read(w_read(reg[r]));
        case 6:
            pc += WORD;
            return b_read(reg[r] + w_read(pc - WORD));
        case 7:
            pc += WORD;
            return b_read(b_read(reg[r] + w_read(pc - WORD)));
        default:
            trace(ERROR, "Address mode is higher than 7! Exiting...");
            exit(17);
    }
}

void write_barg3(byte num, word w) { // num to write and command
    int r = (w & 7); // command register
    int mode = ((w >> 3) & 7); // mode
    byte ans = (0 | num);
    if(num >= 0200) {
        ans += 0xFF00; //если последний "знаковый" бит 1, то первые 8 бит тоже 1
        trace(ERROR, "ss = %d", num);
    }
    //trace(TRACE, "word %06o, mode %d, register %d\n", w, mode, r);
    switch(mode) {
        case 0:
            reg[r] = ans;
            break;
        case 1:
            b_write(reg[r], ans);
            break;
        case 2:
            b_write(reg[r], ans);
            pc += WORD;
            break;
        case 3:
            b_write(w_read(reg[r]), ans);
            pc += WORD;
            break;
        case 4:
            pc -= WORD;
            //trace(ERROR, "pc is %06o\n", pc);
            b_write(reg[r], ans);
            break;
        case 5:
            pc -= WORD;
            b_write(w_read(reg[r]), ans);
            break;
        case 6:
            pc += WORD;
            b_write(reg[r] + w_read(pc - WORD), ans);
            break;
        case 7:
            pc += WORD;
            b_write(w_read(reg[r] + w_read(pc - WORD)), ans);
            break;
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
    trace(TRACE, "      r%d,r%d                  R%d=%06o, R%d=%06o", ((w >> 6) & 7), (w & 7), ((w >> 6) & 7) , ss, (w & 7), dd);
    write_warg3(ss + dd, w);
    if (ss + dd == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (ss + dd > 32768) {
        N = 1;
    } else {
        N = 0;
    }
    if ((int)(ss + dd) > 65536) {
        C = 1;
    } else {
        C = 0;
    }
    if (ss > 32768 && dd > 32768) {
        V = 1;
    } else {
        V = 0;
    }
}
void do_mov(word w) {
    word ss = get_warg3(w >> 6);
    if (((w >> 6) & 7) == 7) {
        trace(TRACE, "      #%06o,r%d             [%06o]=%06o", ss, (w & 7), reg[((w >> 6) & 7)] - WORD, ss);
    } else {
        trace(TRACE, "      r%d,r%d                  R%d=%06o", ((w >> 6) & 7), (w & 7), ((w >> 6) & 7), ss);
    }
    write_warg3(ss, w);
    V = 0;
    if (ss == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (ss > 32768) {
        N = 1;
    } else {
        N = 0;
    }
}
void do_nothing(word w) {
    exit(020);
}

void do_sob(word w) {
    word r = (w >> 6) & 7; // номер регистра
    reg[r] -= 1;
    if (reg[r] != 0) {
        pc -= WORD * (w & 63);
    }
}

void do_clr(word w) {
    write_warg3(0, w);
    N = V = C = 0;
    Z = 1;
}
void do_movb(word w) {
    byte ss = get_barg3(w >> 6);
    if (((w >> 6) & 7) == 7) {
        trace(TRACE, "      #%06o,r%d             [%06o]=%06o", ss, (w & 7), reg[((w >> 6) & 7)] - WORD, ss);
    } else {
        trace(TRACE, "      r%d,r%d                  R%d=%06o", ((w >> 6) & 7), (w & 7), ((w >> 6) & 7), ss);
    }
    write_barg3(ss, w);
    V = 0;
    if (ss == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (ss > 32768) {
        N = 1;
    } else {
        N = 0;
    }
}

void do_br(word w) {
    char offset = (char)(w & 0x00FF);
    pc += WORD * offset;
}

void do_ccc(word w) {
    N = Z = V = C = 0;
}

void do_scc(word w) {
    N = Z = V = C = 1;
}

void do_sec(word w) {
    C = 1;
}

void do_sev(word w) {
    V = 1;
}

void do_sez(word w) {
    Z = 1;
}

void do_sen(word w) {
    N = 1;
}

void do_clc(word w) {
    C = 0;
}

void do_clv(word w) {
    V = 0;
}

void do_clz(word w) {
    Z = 0;
}

void do_cln(word w) {
    N = 0;
}

void do_nop(word w) {
    int a = malloc(sizeof(int));
    free(a); // просто бесполезная операция
}

void do_jmp(word w) {
    pc = get_warg3(w);
}

void do_beq(word w) {
    if (Z == 1) {
        do_br(w);
    }
}

void do_bge(word w) {
    if ((N ^ V) == 0) {
        do_br(w);
    }
}

void do_bgt(word w) {
    if (Z == 0 || (N ^ V) == 0) {
        do_br(w);
    }
}

void do_bcc(word w) {
    if (C == 0) {
        do_br(w);
    }
}

void do_bcs(word w) {
    if (C == 1) {
        do_br(w);
    }
}

void do_bne(word w) {
    if (C == 1) {
        do_br(w);
    }
}

void do_blt(word w) {
    if ((N ^ V) == 1) {
        do_br(w);
    }
}

void do_ble(word w) {
    if (Z == 1 || (N ^ V) == 1) {
        do_br(w);
    }
}

void do_bpl(word w) {
    if (N == 0 && Z == 0) {
        do_br(w);
    }
}

void do_bmi(word w) {
    if (N == 1) {
        do_br(w);
    }
}

void do_bhi(word w) {
    if (C == 0 || Z == 0) {
        do_br(w);
    }
}

void do_blos(word w) {
    if (C == 1 || Z == 1) {
        do_br(w);
    }
}

void do_bhis(word w) {
    if (C == 0) {
        do_br(w);
    }
}

void do_blo(word w) {
    if (C == 1) {
        do_br(w);
    }
}

void do_cmp(word w) { // возможно, работает криво V, C
    word a = get_warg3(w);
    word b = get_warg3(w >> 6);
    if ((a - b) == 0) {
        Z = 1;
        N = 0;
    } else if ((a - b) < 0) {
        Z = 0;
        N = 1;
    }
    if (a > 32768 && b > 32768) {
        V = 0;
    }
    if (a > 32768 || b > 32768) {
        C = 0;
    }
}

void do_cmpb(word w) { // возможно, работает криво V, C
    byte a = get_barg3(w);
    byte b = get_barg3(w >> 6);
    if ((a - b) == 0) {
        Z = 1;
        N = 0;
    } else if ((a - b) < 0) {
        Z = 0;
        N = 1;
    }
    if (a > 255 && b > 255) {
        V = 0;
    }
    if (a > 255 || b > 255) {
        C = 0;
    }

}

void do_tst(word w) {
    if (get_warg3(w) == 0) {
        Z = 1;
        N = 0;
    } else if (get_warg3(w) < 0) {
        N = 1;
        Z = 0;
    }
    V = 0;
    C = 0;
}

void do_tstb(word w) {
    byte b = get_barg3(w);
    if (b == 0) {
        Z = 1;
        N = 0;
    } else if ((b & 128) == 128) {
        N = 1;
        Z = 0;
    }
    V = 0;
    C = 0;
    trace(INFO, "   [%06o]       ", b);
}

Command cmd[] = {
        {0170000, 0010000, "mov", do_mov},
        {0170000, 0110000, "movb", do_movb},
        {0170000, 0060000, "add", do_add},
        {0170000, 0020000, "cmp", do_cmp},
        {0170000, 0120000, "cmpb", do_cmpb},
        {0177000, 0077000, "sob", do_sob},
        {0177000, 0005000, "clr", do_clr},
        {0xFF00, 0x0100, "br", do_br},
        {0xFF00, 0x0200, "bne", do_bne},
        {0xFF00, 0x0300, "beq", do_beq},
        {0xFF00, 0x0400, "bge", do_bge},
        {0xFF00, 0x0500, "blt", do_blt},
        {0xFF00, 0x0600, "bgt", do_bgt},
        {0xFF00, 0x0700, "ble", do_ble},
        {0xFF00, 0x8000, "bpl", do_bpl},
        {0xFF00, 0x8100, "bmi", do_bmi},
        {0xFF00, 0x8200, "bhi", do_bhi},
        {0xFF00, 0x8300, "blos", do_blos},
        {0xFF00, 0x8600, "bcc", do_bcc},
        {0xFF00, 0x8600, "bhis", do_bhis},
        {0xFF00, 0x8700, "bcs", do_bcs},
        {0xFF00, 0x8700, "blo", do_blo},
        {0177700, 0005700, "tst", do_tst},
        {0177700, 0105700, "tstb", do_tstb},
        {0177700, 0000100, "jmp", do_jmp},
        {0177777, 0000000, "halt", do_halt},
        {0177777, 0000240, "nop", do_nop},
        {0177777, 0000241, "clc", do_clc},
        {0177777, 0000242, "clv", do_clv},
        {0177777, 0000244, "clz", do_clz},
        {0177777, 0000250, "cln", do_cln},
        {0177777, 0000257, "ccc", do_ccc},
        {0177777, 0000261, "sec", do_sec},
        {0177777, 0000262, "sev", do_sev},
        {0177777, 0000264, "sez", do_sez},
        {0177777, 0000270, "sen", do_sen},
        {0177777, 0000277, "scc", do_scc},
        {0177777, 0000001, "nothing", do_nothing}, //  резервная команда
        {0000000, 0000000, "unknown", do_nothing} // команда конца списка, есть пока не доделан код команд
};
