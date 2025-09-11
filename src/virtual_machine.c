#include <stdlib.h>
#include "virtual_machine.h"
#include "segment_table.h"
#include "error_handler.h"


VirtualMachine* createVm(int codeSegmentSize,char *fileContent){
    VirtualMachine* virtualM = (VirtualMachine*) malloc(sizeof(VirtualMachine));
    virtualM->memory = (char*) malloc(MEMORY_SIZE);

    initSegmentTable(&virtualM->segment_table);
    addSegment(&virtualM->segment_table, codeSegmentSize);
    addSegment(&virtualM->segment_table, MEMORY_SIZE - codeSegmentSize);//we add the data segment here, we'll have to change it when we add more segments

    /**
     * some entity initialize segment table
     * virtual machine will use it to set 
     * CodeSegment & DataSegment
    */
    setMemoryContent(virtualM, fileContent, codeSegmentSize);

    vmSetUp(virtualM, 0, 1); //assume code segment is 0 and data segment is 1 for now

    return virtualM;
}

/**
 * faltan 2 inyecciones de parametros para determinar 
 * a que segmentos pertenecen CS y DS
 * 
 * por el momento asume 0 y 1
 */

void vmSetUp (VirtualMachine* virtualM, int csSegment, int dsSegment) {
    int *reg = virtualM->registers;

    int cs = csSegment;
    int ds = dsSegment;

    reg[CS] = cs << 16; 
    reg[DS] = ds << 16;

    reg[IP] = reg[CS];
}

void setMemoryContent(VirtualMachine* virtualM, char* fileContent, int contentSize) {
    if (contentSize > MEMORY_SIZE) {
        error_handler.segmentationFault();
    }
    int address =transformLogicalAddress(virtualM->segment_table, virtualM->registers[CS]); //revise
    for (int i = address; i < contentSize; i++)
        virtualM->memory[i] = fileContent[i];
}

void releaseVm (VirtualMachine* virtualM) {
    free(virtualM->memory);
    free(virtualM);
}