#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "segment_table.h"

#include "instructions.h"

#include "common_registers.h"

#include "arguments_parsing.h"

#define DEFAULT_MEMORY_SIZE 16384 

typedef struct VirtualMachine {
    unsigned char* memory;
    int registers[32];
    DST segment_table;

    p_instruction instructions[32];

    char mode;
} VirtualMachine;


VirtualMachine* buildVm(arguments* args, int sizes[]);

/**
 * Instantiates the virtual machine and allocates 2^10 * 16 bytes in memory (*memory).
 * Creates the corresponding segments.
 * 
 * @return A pointer to the newly created VirtualMachine.
 */
void createVm(VirtualMachine* virtualM, int sizes[], int memorySize, int entryPoint, char* codeSegmentContent, char* constSegmentContent, char** paramSegmentContent, int paramsSize);

/**
 * Sets the parameters content in memory, adding at the end the pointers to each parameter.
 */
void setParamContentInMemory(VirtualMachine* virtualM, int memorySize, char** paramsContent, int paramSegmentSize, int paramsSize);

/**
 * Builds the virtual machine from the provided registers and segments of a vmi file.
 */
void restoreVm(VirtualMachine* virtualM, arguments* args, char* fileContent, int regs[], int segs[]);

/**
 * Initializes the segment table registers, IP and SS.
 */
void setSTRegisters(VirtualMachine* virtualM, int reg[], int entrypoint, int paramsSize);

void vmSetUp(VirtualMachine*, char);

void setMemoryContent(VirtualMachine*, int, unsigned char*, int, int);

void releaseVm(VirtualMachine*);

#endif
