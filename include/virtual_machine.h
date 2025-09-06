#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "segment_table.h"

#include "instructions.h"

#include "common_registers.h"

#define MEMORY_SIZE 16384 

typedef struct VirtualMachine {
    char* mem;
    int reg[32];
    // POSIBLE CAMBIO
    ST table_seg[8];  // TSS *table_segs
    /**
     * Se podria llegar a tratar internamente
     * como una pila, donde al agregar un nuevo
     * segmento suma el valor del anterior
     * teniendo un "tope" 
     * 
     * TSS->add( size ) : return TS
     * TSS->getTs( indice ) : TS
     */


    p_instruction instructions[32];
} VirtualMachine;

/**
 * - instancia la vm, reserva 2^10*16 en memoria (*mem)
 * - crea los correspondientes segmentos
 * 
 * @return VirtualMachine*
 */
VirtualMachine* createVm (int codeSegmentSize );

/**
 * inicializa los registros CS, DS, IP
 */
void vmSetUp (VirtualMachine*);

#endif
