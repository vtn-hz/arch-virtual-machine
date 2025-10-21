#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vm_image.h"

#include "virtual_machine.h"

#include "segment_table.h"

#include "vm_mode.h"

#include "error_handler.h"

#include "utils.h"

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

    char printBigEndian[2];
    
    toBigEndian(printBigEndian, size, 2);

    fwrite(printBigEndian, 1, 2, vmi);
}

void saveReg(FILE* vmi, VirtualMachine* vm) {
    char printBigEndian[4];

    for (int i = 0; i < 32; i++) {    
        toBigEndian(printBigEndian, vm->registers[i], 4);
        fwrite(printBigEndian, 1, 4, vmi);
    }
}

void saveTab(FILE* vmi, VirtualMachine* vm) {
    DST st = vm->segment_table;
    char printBigEndian[4];
    int segments[8];
    
    for (int i = 0; i < 8; i++) {
        if (i >= st.counter) 
            segments[i] = -1;
        else {
            segments[i] = st.descriptors[i].base << 16;
            segments[i] |= st.descriptors[i].size;
        }
    }

    for (int i = 0; i < 8; i++) {    
        toBigEndian(printBigEndian, segments[i], 4);
        fwrite(printBigEndian, 1, 4, vmi);
    }
}

void saveMem(FILE* vmi, VirtualMachine* vm, unsigned short size) {
    // printf(">>>%08X<<<\n", size);
    fwrite(vm->memory, 1, size*1024, vmi);
}
