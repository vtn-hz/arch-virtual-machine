#ifndef IO_SYSTEM_CALLS_H
#define IO_SYSTEM_CALLS_H

typedef void (*writeFunc)(int);

int readDecimal();
int readChar();
int readOctal();
int readHex();
int readBinary();

void writeDecimal(int value);
void writeChar(int value);
void writeOctal(int value);
void writeHex(int value);
void writeBinary(int value);

void prepareDisplays(int, writeFunc*, int*);

#endif