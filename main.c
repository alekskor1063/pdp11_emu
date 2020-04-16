#include <stdio.h>
#define 64 * 1024 MEMSIZE

typedef unsigned char byte;
typedef unsigned short int word;
typedef word Address;

void b_write (Address adr, byte b);
void w_write (Address adr, word w);

byte b_read (Address adr);
word w_read (Address adr);

word mem[MEMSIZE]; // memory

int main() {

    return 0;
}
