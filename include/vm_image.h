#ifndef VM_IMAGE_H
#define VM_IMAGE_H

#include "arguments_parsing.h"

typedef struct VirtualMachine VirtualMachine;

void buildImage(VirtualMachine* vm, arguments args);

void arrToChars(int* intVec, int vecSize, char * charVec);

void saveHead(FILE* vmi, unsigned short size);

void saveReg(FILE* vmi, VirtualMachine* vm);

void saveTab(FILE* vmi, VirtualMachine* vm);

void saveMem(FILE* vmi, VirtualMachine* vm, unsigned short size);


#endif
