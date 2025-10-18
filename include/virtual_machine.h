#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "segment_table.h"

#include "instructions.h"

#include "common_registers.h"

#define DEFAULT_MEMORY_SIZE 16384 

typedef struct VirtualMachine {
    unsigned char* memory;
    int registers[32];
    DST segment_table;

    p_instruction instructions[32];

    char mode;
} VirtualMachine;

/**
 * Instantiates the virtual machine and allocates 2^10 * 16 bytes in memory (*memory).
 * Creates the corresponding segments.
 * 
 * @return A pointer to the newly created VirtualMachine.
 */
void createVm(VirtualMachine* virtualM, int sizes[], int entryPoint, char* codeSegmentContent, char* constSegmentContent, char* paramSegmentContent, int paramsSize);

void buildVm(VirtualMachine* virtualM, arguments* args, char* fileContent, int regs[], int segs[]);

/**
 * Initializes the segment table registers and IP.
 */
void setSTRegisters(VirtualMachine* virtualM, int reg[], int entrypoint);

void vmSetUp(VirtualMachine*);

void setMemoryContent(VirtualMachine*, char*, int);

void releaseVm(VirtualMachine*);

#endif
