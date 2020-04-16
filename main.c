#include <stdio.h>
#include <assert.h>

#define MEMSIZE 65536

typedef unsigned char byte;
typedef unsigned short int word;
typedef word adr;

byte mem[MEMSIZE]; // memory

void b_write (adr a, byte b) {
    mem[a] = b;
}
void w_write (adr a, word w) {
    mem[a] = (byte)w;
    mem[a+1] = (byte)(w >> 8);
}

byte b_read (adr a) {
    return mem[a];
}
word w_read (adr a) {
    word w = (word)mem[a];
    w = w | (word)mem[a+1] << 8; // еще раз спросить про это
    return w;
}

void testmem() {
    adr a = 4;
    byte b1 = 0xff, b0 = 0xff;
    word w = 0xffff;
    b_write (a, b0);
    b_write (a+1, b1);
    word wres = w_read (a);
    printf("%04hx=%02hhx%02hhx\n", wres, b1, b0);
    assert(wres == w);
}

int main() {
    testmem();
    return 0;
}
