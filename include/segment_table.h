#ifndef SEGMENT_TABLE_H
#define SEGMENT_TABLE_H
#define DST_MAX 8

typedef struct {
    unsigned short base;
    unsigned short size;
} ST;

typedef struct {
    ST descriptors[DST_MAX];
    unsigned short counter;
    int available_memory;
} DST;

void createSegmentTable(DST* table, int memorySize);

void initSegmentTable(DST* table, int sizes[], int reg[]);

/**
 * Converts a logical address to a physical address using
 * the base of the corresponding segment from tableSegment.
 * 
 * @return The corresponding physical address (int).
 */
int transformLogicalAddress(DST tableSegment, int logicalAddress);

/**
 * Checks whether access to a given logical address is permitted.
 * 
 * @return 1 if access is permitted, 0 otherwise (segmentation fault).
 */ 
int isLogicalAddressValid(DST tableSegment, int logicalAddress);

void addSegment(DST* table, unsigned short size);

int memorySizeLeft(DST table);

#endif
