#include <stdio.h>
#include <stdlib.h>
#include "segment_table.h"
#include "error_handler.h"
#include "virtual_machine.h"

void createSegmentTable(DST* table) {
    table->counter = 0;
    for (int i = 0; i < DST_MAX; i++) {
        table->descriptors[i].base = -1;
        table->descriptors[i].size = -1;
    }
}

void initSegmentTable(DST* table, int sizes[], int registers[]) { //could be called by createVm or the function that extracts data from files
    int reg[] = {-1};

    for(int i = 0; i < DST_MAX; i++)
        if(sizes[i] > 0){
            addSegment(table, sizes[i]);
            reg[i] = (table->counter -1) << 16; // segment number in high bytes. again, order: param, const, code, data, extra, stack (same as to be saved in memory)
        }

    // call function that sets registers accordingly
}

static int calcLogicToPhysical(DST table, int logicalAddress) {
    short int segment = (logicalAddress >> 16) & 0xFFFF;
    short int offset = logicalAddress & 0xFFFF;
    
    if (segment < 0 || segment >= table.counter)
        return -1;
    
    int base = table.descriptors[segment].base;
    int size = table.descriptors[segment].size;

    if(base == -1 || size == -1)
        return -1; // segment not initialized

    int physicalAddress = base + offset;
    if (physicalAddress < base || physicalAddress >= base + size) 
        return -1; // offset out of bounds
    
    return physicalAddress;
}

int transformLogicalAddress(DST table, int logicalAddress) {
    int physicalAddress = calcLogicToPhysical(table, logicalAddress);
    if (physicalAddress == -1) 
        error_handler.segmentationFault(logicalAddress);
    
    return physicalAddress;
}

int isLogicalAddressValid(DST table, int logicalAddress) {
    return calcLogicToPhysical(table, logicalAddress) != -1;
}

void addSegment(DST* table, unsigned short size) {
    unsigned short base = 0;
    if (table->counter == DST_MAX) 
        error_handler.buildError("Error: segment table lleno"); // no more space for new segments
    
    if (table->counter > 0) {
        ST* lastSegment = &table->descriptors[table->counter - 1];
        base = lastSegment->base + lastSegment->size;
    }

    if (base + size > DEFAULT_MEMORY_SIZE) 
        error_handler.buildError("Error: {base: %d, size: %d} memoria solicitada no disponible", base, size); // not enough memory   

    table->descriptors[table->counter].base = base;
    table->descriptors[table->counter].size = size;
    table->counter++;
}

int memorySizeLeft(DST table) {
    int usedMemory = table.descriptors[table.counter-1].base + table.descriptors[table.counter-1].size;
    return DEFAULT_MEMORY_SIZE - usedMemory;
}
