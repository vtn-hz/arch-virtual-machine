#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "segment_table.h"

#include "instructions.h"

#include "common_registers.h"

#define MEMORY_SIZE 16384 

typedef struct VirtualMachine {
    char* memory;
    int registers[32];
    DST segment_table;

    p_instruction instructions[32];
} VirtualMachine;

/**
 * - instancia la vm, reserva 2^10*16 en memoria (*memory)
 * - crea los correspondientes segmentos
 * 
 * @return VirtualMachine*
 */
VirtualMachine* createVm (int codeSegmentSize, char *fileContent);

/**
 * inicializa los registros CS, DS, IP
 */
void vmSetUp (VirtualMachine*, int csSegment, int dsSegment);

void setMemoryContent(VirtualMachine*, char*, int);

void releaseVm(VirtualMachine*);

#endif
