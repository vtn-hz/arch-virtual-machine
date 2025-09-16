#include "system_calls.h"
#include "virtual_machine.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    int v = strtol(binStr, NULL, 2);
    int s = 32 - log2(v);
    return binStr[0] == '0' ? v : spreadSign(v, s); // if leading bit isn't 0, make it negative
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
    printf("0o");
    printf("%o", value);
}

void writeHex(int value) {
    printf("0x");
    printf("%X", value);
}

void writeBinary(int value) {
    int sign = 0; // adds leading bit(s) with sign
    printf("0b");
    for (int i = value ? log2(abs(value)) + sign : 0; i >= 0; i--) {
        putchar((value & (1 << i)) ? '1' : '0');
    }
}

void prepareDisplays(int mode, writeFunc funcs[], int *count) {
    if (mode & 0b10000) funcs[(*count)++] = writeBinary;
    if (mode & 0b01000) funcs[(*count)++] = writeHex;
    if (mode & 0b00100) funcs[(*count)++] = writeOctal;
    if (mode & 0b00010) funcs[(*count)++] = writeChar;
    if (mode & 0b00001) funcs[(*count)++] = writeDecimal;
}