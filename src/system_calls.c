#include "system_calls.h"
#include "virtual_machine.h"

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
    printf("%d", value);
}

void writeChar(int value) {
    if (value >= 32 && value < 128)
        printf("%c", (char)value);
    else
        printf(".");
}

void writeOctal(int value) {
    printf("%o", value);
}

void writeHex(int value) {
    printf("%X", value);
}

void writeBinary(int value) {
    for (int i = (sizeof(value) - 2) * 8 - 1; i >= 0; i--) {
        putchar((value & (1 << i)) ? '1' : '0');
    }
}

void prepareDisplays(int mode, writeFunc funcs[], int *count) {
    if (mode & 0b00001) funcs[(*count)++] = writeDecimal;
    if (mode & 0b00010) funcs[(*count)++] = writeChar;
    if (mode & 0b00100) funcs[(*count)++] = writeOctal;
    if (mode & 0b01000) funcs[(*count)++] = writeHex;
    if (mode & 0b10000) funcs[(*count)++] = writeBinary;
}