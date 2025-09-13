#include <stdio.h>
#include <stdlib.h>
#include "segment_table.h"
#include "error_handler.h"
#include "virtual_machine.h"


void initSegmentTable(DST* table) {
    table->counter = 0;
    for (int i = 0; i < DST_MAX; i++) {
        table->descriptors[i].base = 0;
        table->descriptors[i].size = 0;
    }
}

static int calcLogicToPhysical(DST table, int logicalAddress) {
    short int segment = (logicalAddress >> 16) & 0xFFFF; // extract segment (upper 8 bits)
    short int offset = logicalAddress & 0xFFFF;        // extract offset (lower 8 bits)
    
    if (segment < 0 || segment >= table.counter)
        return -1;
    
    int base = table.descriptors[segment].base;
    int size = table.descriptors[segment].size;

    int physicalAddress = base + offset;
    if (physicalAddress < base || physicalAddress >= base + size) 
        return -1; // offset out of bounds
    
    return physicalAddress;
}

int transformLogicalAddress (DST table, int logicalAddress) {
    int physicalAddress = calcLogicToPhysical(table, logicalAddress);
    if(physicalAddress == -1) 
        error_handler.segmentationFault("logical address 0x%08X", logicalAddress);
    
    return physicalAddress;
}

int isLogicalAddressValid (DST table, int logicalAddress) {
    return calcLogicToPhysical(table, logicalAddress) != -1;
}

void addSegment(DST* table, unsigned short size) {
    unsigned short base;
    if (table->counter == DST_MAX) 
        error_handler.segmentationFault( NO_ERROR_CONTEXT ); // no more space for new segments
    
    if (table->counter > 0) {
        ST* lastSegment = &table->descriptors[table->counter - 1];
        base = lastSegment->base + lastSegment->size;
    }

    if (base + size > MEMORY_SIZE) 
        error_handler.segmentationFault( NO_ERROR_CONTEXT ); // not enough memory   

    table->descriptors[table->counter].base = base;
    table->descriptors[table->counter].size = size;
    table->counter++;
}

int memorySizeLeft(DST table) {
    int usedMemory = table.descriptors[table.counter-1].base + table.descriptors[table.counter-1].size;
    return MEMORY_SIZE - usedMemory;
}
