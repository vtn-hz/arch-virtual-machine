#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "segment_table.h"

typedef struct {
    char* mem;
    int reg[32];
    TS table_seg[8];
    // mnemonics
} VirtualMachine;

#endif