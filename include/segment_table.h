#ifndef SEGMENT_TABLE_H
#define SEGMENT_TABLE_H
#define DST_MAX 8

typedef struct {
    unsigned short base;
    unsigned short size;
} ST;

typedef struct{
    ST descriptors[DST_MAX];
    unsigned short counter;
} DST;

void initSegmentTable(DST* tableSegment);
/**
*   Devuelve recibe la memoria logica
*   aplica la base correspondiente enviada por el tableSegment
*   pasandola de logica a fisica
*   @return int
*/
int transformLogicalAddress (DST tableSegment, int logicalAddress);

/**
*   Devuelve 1 o 0 (bool) acorde a si tiene el accesso permitido 
*   a dicha memoria o no (Segmentation Fault)
*   @return bool
*/ 
int isLogicalAddressValid (DST tableSegment, int logicalAddress);

void addSegment(DST* table, unsigned short size);

int memorySizeLeft(DST table);

#endif
