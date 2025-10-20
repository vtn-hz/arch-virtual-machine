#include <stdio.h>
#include <stdlib.h>
#include "segment_table.h"
#include "error_handler.h"
#include "virtual_machine.h"

void createSegmentTable(DST* table, int memorySize) {
    table->counter = 0;
    for (int i = 0; i < DST_MAX; i++) {
        table->descriptors[i].base = -1;
        table->descriptors[i].size = -1;
    }
    table->available_memory = memorySize*1024; // in bytes
}

void initSegmentTable(DST* table, int sizes[], int reg[]) { //could be called by createVm or the function that extracts data from files

    for(int i = 0; i < DST_MAX; i++)
        if(sizes[i] > 0) {
            /* good job here, i think it works */ 
            addSegment(table, sizes[i]);
            reg[i] = ((table->counter) -1 ) << 16; // segment number in high bytes. again, order: param, const, code, data, extra, stack (same as to be saved in memory)
        }
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

    // here maybe we have to add available_memory to DST struct to check if we have enough memory (now is variable) mari: like it, done
    if (base + size > table->available_memory) 
        error_handler.buildError("Error: {base: %d, size: %d} memoria solicitada no disponible", base, size); // not enough memory   

    table->descriptors[table->counter].base = base;
    table->descriptors[table->counter].size = size;
    table->counter++;
}

int memorySizeLeft(DST table) {
    // available_memory in struct is required
    int usedMemory = table.descriptors[table.counter-1].base + table.descriptors[table.counter-1].size;
    return table.available_memory - usedMemory;
}
