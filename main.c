#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "pdp.h"

byte mem[MEMSIZE]; // memory
word reg[8]; // register
int trace_type = INFO; // default output

void trace(int type, char * str, ...) { // степень трассировки, печатаемая строка, переменные
    if (type > trace_type) {
        return;
    } else {
        va_list ap;
        va_start(ap, str);
        vprintf(str, ap);
        va_end(ap);
    }
}

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

void load_text(FILE * fp){
    adr address;
    unsigned short int n;
    byte k = 0x00;
    int i;
    while(fscanf(fp, "%04hx%04hx", &address, &n) != EOF) {
        for (i = 0; i < n; i++) {
            fscanf(fp, "%02hhx", &k);
            b_write(address + i, k);
            //trace(DEBUG, "scaned %d, i = %d, n = %d\n", k, i, n);
        }
    }
}

void mem_dump(adr address, int n){
    for (int i = 0; i < n; i += WORD) {
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
    mem_dump(0x40, 0x4);
    mem_dump(0x200, 0x12);
}

void testmem3(FILE * file) {
    load_text(file);
    mem_dump(0x40, 0xC);
    mem_dump(0x200, 0x26);
}
void run_program(FILE * file) {
    load_text(file);
    run();
}

int main(int argc, char* argv[]) {
    FILE * fp;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--error") == 0) {
            trace_type = ERROR;
        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--info") == 0) {
            trace_type = INFO;
        } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--trace") == 0) {
            trace_type = TRACE;
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) {
            trace_type = DEBUG;
        } else {
            if((fp = fopen(argv[i], "r")) == NULL) {
                printf ("File is not found or unreadable.\n");
                exit(1);
            } // check file
        }
    }
    //trace(ERROR, "Trace type set: %d\n", trace_type);
    run_program(fp);
    fclose(fp);
    return 0;
}
