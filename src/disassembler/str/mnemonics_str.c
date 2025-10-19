#include "mnemonics_str.h"
#include <stddef.h>

const char* MNEMONICS_STR[32] = {
    "SYS",  // 0x00
    "JMP",  // 0x01
    "JZ",   // 0x02
    "JP",   // 0x03
    "JN",   // 0x04
    "JNZ",  // 0x05
    "JNP",  // 0x06
    "JNN",  // 0x07
    "NOT",  // 0x08
    NULL,   // 0x09
    NULL,   // 0x0A
    "PUSH", // 0x0B
    "POP",  // 0x0C
    "CALL", // 0x0D
    "RET",  // 0x0E
    "STOP", // 0x0F
    "MOV",  // 0x10
    "ADD",  // 0x11
    "SUB",  // 0x12
    "MUL",  // 0x13
    "DIV",  // 0x14
    "CMP",  // 0x15
    "SHL",  // 0x16
    "SHR",  // 0x17
    "SAR",  // 0x18
    "AND",  // 0x19
    "OR",   // 0x1A
    "XOR",  // 0x1B
    "SWAP", // 0x1C
    "LDL",  // 0x1D
    "LDH",  // 0x1E
    "RND",  // 0x1F
};
