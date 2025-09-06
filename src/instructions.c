#include <stdio.h>
#include <stdlib.h>
#include <instructions.h>

void getInstruction(FILE *, char *, char *, char *, int *);
void getOperands(FILE *, char*, char*, char, char, int);

void instructor() {
    FILE * vmx25 = fopen("sample.vmx", "rb");
    if (!vmx25) {
        printf("No se pudo abrir el archivo\n");
        // call error handler
    } else {
        // advance pointer to instruction
        fseek(vmx25, 8, 0); // hardcoded (8, 11, 17)

        char cod, tp1, tp2, op1, op2;
        int n;

        getInstruction(vmx25, &cod, &tp1, &tp2, &n);
        getOperands(vmx25, &op1, &op2, tp1, tp2, n);
        // process information and loop

        //printf("mnm: %02X\ntp1: %02X\ntp2: %02X\nop#: %d\n", cod, tp1, tp2, n);
    }

    fclose(vmx25);
}

void getInstruction(FILE * vmx, char * cod, char * tp1, char * tp2, int * n) {
    char c;
    fread(&c, 1, 1, vmx);
    *cod = c & 0x1F; // 00011111

    c >>= 4;
    if (c & 0x01) {
        *tp1 = c & 0x03;
        *tp2 = c >> 2 & 0x03;
        *n = 2;
    } else {
        *tp1 = c >> 2 & 0x03;
        *tp2 = 0;
        *n = *tp1 > 0;
    }
}

void getOperands(FILE * vmx, char * op1, char * op2, char tp1, char tp2, int n) {
    int oparr[2], optarr[2] = {tp1, tp2};

    for (int i = n; i > 0; i--) {
        char buf[3] = {0};
        int opaux = 0;

        fread(buf, 1, optarr[i-1], vmx);
        for (int j = 0; j < optarr[i-1]; j++) {
            opaux <<= 8;
            opaux |= buf[j];
        }

        //printf("[ %06X ]\n", opaux);
        oparr[i-1] = opaux;
    }

    printf("op1: %06X\nop2: %06X\n", oparr[0], oparr[1]);
}