//
// Created by light on 17.04.2020.
//

#ifndef PDP11_EMU_PDP11_H
#define PDP11_EMU_PDP11_H

#endif //PDP11_EMU_PDP11_H

#define MEMSIZE 65536

typedef unsigned char byte;
typedef unsigned short int word;
typedef word adr;

extern byte mem[MEMSIZE];

void b_write (adr a, byte b);
void w_write (adr a, word w);
byte b_read (adr a);
word w_read (adr a);
