#include <stdio.h>
#include <stdlib.h>
#include <instructions.h>

void instructor() {
    char c;

    FILE * vmx25 = fopen("test/sample.vmx", "rb");
    
    if (!vmx25) {
        printf("No se pudo abrir el archivo\n");
        // call error handler
    } else {
        // advance pointer to instruction
        fseek(vmx25, sizeof(char)*8, 0); // 50_16 = 01010000_2

        fread(&c, sizeof(char), 1, vmx25);
        char cod = c & 0x0F; // 00001111

        // according to the mnemonic, change the operand types (and shifts)
        c >>= 4;
        char op2 = c & 0x03; // 00000011
        c >>= 2;
        char op1 = c & 0x03;

        // save operands
        printf("%02X, %02X, %02X\n", cod, op1, op2);
    }

    fclose(vmx25);
}