#include "system_calls.h"
#include <stdio.h>
#include <stdlib.h>

int readDecimal() {
    int v; 
    scanf("%d", &v); 
    return v;
}

int readChar() {
    char c; 
    scanf("%c", &c); 
    return (int)c;
}

int readOctal() {
    int v;
    scanf("%o", &v); 
    return v;
}

int readHex() {
    int v; 
    scanf("%x", &v); 
    return v;
}

int readBinary() {
    char binStr[33];
    scanf("%32s", binStr);
    return (int)strtol(binStr, NULL, 2);
}

void writeDecimal(int value) {
    printf("%d\n", value);
}

void writeChar(int value) {
    printf("%c\n", (char)value);
}

void writeOctal(int value) {
    printf("%o\n", value);
}

void writeHex(int value) {
    printf("%X\n", value);
}

void writeBinary(int value) {
    for (int i = sizeof(value) * 8 - 1; i >= 0; i--) {
        putchar((value & (1 << i)) ? '1' : '0');
    }
    printf("\n");
}
