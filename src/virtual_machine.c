#include <stdlib.h>
#include <stdio.h>

#include "virtual_machine.h"

#include "segment_table.h"

#include "data_access.h"

#include "error_handler.h"


VirtualMachine* createVm(int codeSegmentSize,char *fileContent){
    VirtualMachine* virtualM = (VirtualMachine*) malloc(sizeof(VirtualMachine));
    virtualM->memory = (unsigned char*) malloc(MEMORY_SIZE);

    initSegmentTable(&virtualM->segment_table);
    printf("after init segment table\n"); //debug
    addSegment(&virtualM->segment_table, codeSegmentSize);
    addSegment(&virtualM->segment_table, memorySizeLeft(virtualM->segment_table));//we add the data segment here, we'll have to change it when we add more segments
    /**
     * some entity initialize segment table
     * virtual machine will use it to set 
     * CodeSegment & DataSegment
    */
    printf("before set memory content\n"); //debug
    setMemoryContent(virtualM, fileContent, codeSegmentSize);
    return virtualM;
}

void vmSetUp (VirtualMachine* virtualM, int csSegment, int dsSegment) {
    int *registers = virtualM->registers;

    registers[CS] = csSegment << 16; 
    registers[DS] = dsSegment << 16;

    registers[IP] = registers[CS];

    initializeGetters();
    initializeSetters();
    initializeInstructions(virtualM);
}

void setMemoryContent(VirtualMachine* virtualM, char* fileContent, int contentSize) {
    if (contentSize > MEMORY_SIZE) {
        error_handler.buildError("Error: el tamaño del contenido {%d} excede la memoria disponible", contentSize);
    }
    int address =transformLogicalAddress(virtualM->segment_table, virtualM->registers[CS]); //revise
    printf("Code Segment Base: %d\n", address); //debug
    for (int i = address; i < contentSize; i++)
        virtualM->memory[i] = fileContent[i];
    for (int i=address; i<contentSize; i++)
        printf("%02X ", (unsigned char)virtualM->memory[i]); //debug
}

void releaseVm (VirtualMachine* virtualM) {
    free(virtualM->memory);
    free(virtualM);
}
