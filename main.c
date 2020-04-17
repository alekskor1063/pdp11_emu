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
    //word w = (word)mem[a];
    //w = w | (word)mem[a+1] << 8; // еще раз спросить про это
    word w = ((word)mem[a+1]) << 8;
    w = w | mem[a];
    return w;
}

void load_file(){
    adr address;
    unsigned short int n;
    byte k = 0x00;
    int i;
    int j = 2;
    while(1) {

        j = scanf("%04hx%04hx", &address, &n);
        //printf("%d\n", j);
        if(j != 2){
            break;
        }
        for (i = 0; i < n; i++) {
            scanf("%02hhx", &k);
            b_write(address + i, k);
            //printf("scaned %d, i = %d, n = %d\n", k, i, n);
        }
    }
}

void mem_dump(adr address, int n){
    for (int i = 0; i < n; i += 2) {
        printf("%06o : %06ho\n",address + i, w_read(address + i));
    }
    /*
    if (n % 2 == 1) {
        printf("")
    }
     */
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

void testmem2(){
    load_file();
    mem_dump(0x40, 4);
    mem_dump(0x200, 0x12);
}

int main() {
    testmem2();
    return 0;
}
