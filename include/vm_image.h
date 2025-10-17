#ifndef VM_IMAGE_H
#define VM_IMAGE_H

typedef struct VirtualMachine VirtualMachine;

void buildImage(VirtualMachine* vm);

char* arrToChars(int* intVec, int vecSize);

void saveHead(FILE* vmi, unsigned short size);

void saveReg(FILE* vmi, VirtualMachine* vm);

void saveTab(FILE* vmi, VirtualMachine* vm);

void saveMem(FILE* vmi, VirtualMachine* vm, unsigned short size);


#endif
