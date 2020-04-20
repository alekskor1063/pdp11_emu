//
// Created by light on 17.04.2020.
//

#ifndef PDP11_EMU_PDP11_H
#define PDP11_EMU_PDP11_H

#endif //PDP11_EMU_PDP11_H

#define MEMSIZE 65536

#define ERROR 0
#define INFO 1
#define TRACE 2
#define DEBUG 3

typedef unsigned char byte;
typedef unsigned short int word;
typedef word adr;
#define WORD (sizeof(word) / sizeof(byte))

typedef struct {
    word mask;
    word opcode;
    char* name;
    void (* do_func)(word);
} Command;

extern byte mem[MEMSIZE];
extern word reg[8]; // регистры R0...R7
#define pc reg[7]
extern Command cmd[];
extern int N;
extern int Z;
extern int V;
extern int C;

extern int trace_type; // тип трассировки
void trace(int type, char * str, ...);

void run();
void mem_dump(adr address, int n);
void display();

void b_write (adr a, byte b);
void w_write (adr a, word w);
byte b_read (adr a);
word w_read (adr a);