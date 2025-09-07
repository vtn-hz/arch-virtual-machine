#include <stdlib.h>

#include "virtual_machine.h"

VirtualMachine* createVm ( int codeSegmentSize ) {
    VirtualMachine* virtualM = (VirtualMachine*) malloc( sizeof(VirtualMachine) );
    virtualM->mem = (char*) malloc( MEMORY_SIZE );


    /**
     * some entity initialize segment table
     * virtual machine will use it to set 
     * CodeSegment & DataSegment
     */
    virtualM->table_seg[0].base = 0;
    virtualM->table_seg[0].size = codeSegmentSize;

    virtualM->table_seg[1].base = codeSegmentSize;
    virtualM->table_seg[1].size = MEMORY_SIZE - codeSegmentSize;
    // NO va aca esto, se deberia encargar la entidad ST

    return virtualM;
}

/**
 * faltan 2 inyecciones de parametros para determinar 
 * a que segmentos pertenecen CS y DS
 * 
 * por el momento asume 0 y 1
 */
void vmSetUp (VirtualMachine* virtualM) {
    int *reg = virtualM->reg;

    int cs = 0;
    int ds = 1;

    reg[ CS ] = cs << 16; 
    reg[ DS ] = ds << 16;

    reg[ IP ] = reg[ CS ];
}

void releaseVm (VirtualMachine* virtualM) {
    free(virtualM->mem);
    free(virtualM);
}