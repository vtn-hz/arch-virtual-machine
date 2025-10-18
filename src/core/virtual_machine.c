#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "virtual_machine.h"

#include "vm_mode.h"

#include "segment_table.h"

#include "data_access.h"

#include "error_handler.h"

#include "arguments_parsing.h"

VirtualMachine* initializeVM_fromFile(arguments* args, int sizes[]) {
    VirtualMachine* virtualM = (VirtualMachine*) malloc(sizeof(VirtualMachine));
    
    char* codeSegmentContent;
    int entryPoint;
    char* constSegmentContent;
    int regs[32]; //only for the vmi, kind of wasteful but easier this way
    int segs[8];
    
    getParsed(&codeSegmentContent, &constSegmentContent, args, sizes, &entryPoint, regs, segs);

    virtualM->memory = (unsigned char*) malloc(args->memory_size * 1024);

    if(args->currentVmx){
        createVm(virtualM, sizes, entryPoint, codeSegmentContent, constSegmentContent, args->params, args->paramsAmount);
    }
    else
        buildVm(virtualM, args, codeSegmentContent, regs, segs);

    free(codeSegmentContent);
    free(constSegmentContent); //is it so bad if we do this here?

    return virtualM;
}

void createVm(VirtualMachine* virtualM, int sizes[], int entryPoint, char* codeSegmentContent, char* constSegmentContent, char* paramSegmentContent, int paramsSize) { 
    int reg[8] = {-1};

    if(sizes[0] > 0)
       setParamContentInMemory(virtualM, paramSegmentContent, sizes[0], paramsSize);

    createSegmentTable(&virtualM->segment_table);
    initSegmentTable(&virtualM->segment_table, sizes, reg); 
    setSTRegisters(virtualM, reg, entryPoint);

    vmSetUp(virtualM);

    if(sizes[1] > 0)
        setMemoryContent(virtualM, constSegmentContent, sizes[1]);

    setMemoryContent(virtualM, codeSegmentContent, sizes[2]);
}

void setParamContentInMemory(VirtualMachine* virtualM, char* paramsContent, int paramSegmentSize, int paramsSize) {
    int pointers[paramsSize];
    int previousSize = 0;
    int i;
    char* cad;

    for( i = 0; i < paramsSize; i++){
        pointers[i] = previousSize;
        previousSize+=strlen(&paramsContent[previousSize]) + 1; //+1 for the null terminator
    }

    for( i = 0; i<paramsSize; i++){
        cad = intToString(pointers[i]);
        strcat(paramsContent, cad);
        free(cad);
    }

    setMemoryContent(virtualM, paramsContent, paramSegmentSize);
}
    
void buildVm(VirtualMachine* virtualM, arguments* args, char* fileContent, int regs[], int segs[]) {

    createSegmentTable(&virtualM->segment_table);

    vmSetUp(virtualM);

    for(int i = 0; i < 32; i++)
        virtualM->registers[i] = regs[i];
    
    for(int i = 0; i < DST_MAX; i++){
        virtualM->segment_table.descriptors[i].base = segs[i] >> 16;
        virtualM->segment_table.descriptors[i].size = segs[i] & 0x0000FFFF;
    }
        
    setMemoryContent(virtualM, fileContent, args->memory_size * 1024);
}

void vmSetUp(VirtualMachine* virtualM) {

    virtualM->mode = GO_MODE; // if it was restored may be initilized with DEBUG_MODE

    initializeGetters();
    initializeSetters();
    initializeInstructions(virtualM);
}

void setSTRegisters(VirtualMachine* virtualM, int reg[], int entrypoint) {
    int* registers = virtualM->registers;

    registers[PS] = reg[0];
    registers[KS] = reg[1];
    registers[CS] = reg[2];
    registers[DS] = reg[3];
    registers[ES] = reg[4];
    registers[SS] = reg[5];

    registers[IP] = (registers[CS] & 0xFFFF0000) | (entrypoint & 0x0000FFFF);
}

void setMemoryContent(VirtualMachine* virtualM, char* fileContent, int contentSize) {
    if (contentSize > DEFAULT_MEMORY_SIZE) {
        error_handler.buildError("Error: el tamaño del contenido {%d} excede la memoria disponible", contentSize);
    }
    int address = transformLogicalAddress(virtualM->segment_table, virtualM->registers[CS]);
    
    for (int i = address; i < contentSize; i++)
        virtualM->memory[i] = fileContent[i];
}

void releaseVm(VirtualMachine* virtualM) {
    free(virtualM->memory);
    free(virtualM);
}
