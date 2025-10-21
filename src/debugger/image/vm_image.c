#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vm_image.h"

#include "virtual_machine.h"

#include "segment_table.h"

#include "vm_mode.h"

#include "error_handler.h"

// 
void arrToChars(int* intVec, int vecSize, char* charVec) {
    for (int i = 0; i < vecSize; i++) {
        int value = intVec[i];
        charVec[i * 4]     = (value >> 24) & 0xFF;
        charVec[i * 4 + 1] = (value >> 16) & 0xFF;
        charVec[i * 4 + 2] = (value >> 8)  & 0xFF;
        charVec[i * 4 + 3] = value         & 0xFF;
    }
}

void buildImage(VirtualMachine* vm, arguments args) {
    char* path = args.currentVmi;
    unsigned short size = args.memory_size;

    FILE* vmi = fopen(path, "wb");

    saveHead(vmi, size);
    saveReg(vmi, vm);
    saveTab(vmi, vm);
    saveMem(vmi, vm, size);
    
    fclose(vmi);
}

void saveHead(FILE* vmi, unsigned short size) {
    char ver = 1;
    fwrite("VMI25", 1, 5, vmi);
    fwrite(&ver, 1, 1, vmi);
    fwrite(&size, 2, 1, vmi);
}

void saveReg(FILE* vmi, VirtualMachine* vm) {
    char* printArr = (char*)malloc(32 * 4);
    arrToChars(vm->registers, 32, printArr);

    fwrite(printArr, 1, 4*32, vmi);

    free(printArr);
}

void saveTab(FILE* vmi, VirtualMachine* vm) {
    DST st = vm->segment_table;
    
    int segments[8];
    for (int i = 0; i < 8; i++) // initialization
    segments[i] = -1;
    
    for (int i = 0; i < st.counter; i++) // ldh base
    segments[i] = st.descriptors[i].base << 16;
    for (int i = 0; i < st.counter; i++) // ldl size
    segments[i] |= st.descriptors[i].size;
    
    char * printArr = (char*)malloc(8 * 4);
    arrToChars(segments, 8, printArr);
    
    fwrite(printArr, 1, 4*8, vmi);

    free(printArr);
}

void saveMem(FILE* vmi, VirtualMachine* vm, unsigned short size) {
    fwrite(vm->memory, 1, size, vmi);
}
