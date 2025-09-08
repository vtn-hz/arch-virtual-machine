#include <stdio.h>
#include <stdlib.h>
#include <instructions.h>

void getInstruction(FILE *, char *, char *, char *, int *, int *);
void getOperands(FILE *, int *, int *, char, char, int);

void instructor() {
    FILE * vmx25 = fopen("test/sample.vmx", "rb");
    if (!vmx25) {
        printf("No se pudo abrir el archivo\n");
        // call error handler
    } else {
        fseek(vmx25, 8, 0); // first instruction

        char cod, tp1, tp2;
        int op1, op2, n;

        int flag = 1;
        getInstruction(vmx25, &cod, &tp1, &tp2, &n, &flag);
        while (flag) {
            op1 = op2 = 0;
            getOperands(vmx25, &op1, &op2, tp1, tp2, n);
            // process information
            // printf("mnm: %02X, op1: %08X, op2: %08X, num: %d\n", cod, op1, op2, n);

            getInstruction(vmx25, &cod, &tp1, &tp2, &n, &flag);
        }
    }

    fclose(vmx25);
}

void getInstruction(FILE * vmx, char * cod, char * tp1, char * tp2, int * n, int * flag) {
    char c;
    if (!fread(&c, 1, 1, vmx)) {
        *flag = 0;
    } else {
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
}

void getOperands(FILE * vmx, int * op1, int * op2, char tp1, char tp2, int n) {
    int * oparr[2] = {op1, op2};
    char optarr[2] = {tp1, tp2};

    for (int i = n; i > 0; i--) {
        char buf[3] = {0};
        int opaux = 0;

        fread(buf, 1, optarr[i-1], vmx);
        for (int j = 0; j < optarr[i-1]; j++) {
            opaux <<= 8;
            opaux |= buf[j];
        }

        *oparr[i-1] = opaux;
        // if the value is immediate and negative, the int saves a byte too many (it should be, at most, three)
    }
}