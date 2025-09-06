#include <stdio.h>
#include <stdlib.h>
#include <instructions.h>

void getInstruction(FILE *, char *, char *, char *, int *);

void instructor() {
    FILE * vmx25 = fopen("test/sample.vmx", "rb");
    
    if (!vmx25) {
        printf("No se pudo abrir el archivo\n");
        // call error handler
    } else {
        // advance pointer to instruction
        fseek(vmx25, sizeof(char)*8, 0); // 50_16 = 01010000_2

        char cod, op1, op2;
        int n; // unnecessary?

        getInstruction(vmx25, &cod, &op1, &op2, &n);

        // process information
        printf("Mnm: %02X\nOp1: %02X\nOp2: %02X\nOp#: %d\n", cod, op1, op2, n);
    }

    fclose(vmx25);
}

void getInstruction(FILE * f, char * cod, char * op1, char * op2, int * n) {
    char c;
    fread(&c, sizeof(char), 1, f);
    *cod = c & 0x1F; // 00011111

    c >>= 4;
    if (c & 0x01) {
        *op1 = c & 0x03;
        *op2 = c >> 2 & 0x03;
        *n = 2;
    } else {
        *op1 = c >> 2 & 0x03;
        *op2 = 0;
        *n = *op1 > 0;
    }
}