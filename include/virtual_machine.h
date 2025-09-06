#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "segment_table.h"

typedef struct {
    char* mem;
    int reg[32];
    // POSIBLE CAMBIO
    TS table_seg[8];  // TSS *table_segs
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

#endif