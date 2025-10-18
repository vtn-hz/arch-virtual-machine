#include "io_system_calls.h"

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
