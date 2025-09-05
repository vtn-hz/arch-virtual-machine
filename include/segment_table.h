#ifndef __SEGMENT_TABLE__
#define __SEGMENT_TABLE__

typedef struct {
    unsigned short base;
    unsigned short size;
} ST;


/**
*   Devuelve recibe la memoria logica con su correspondiente offset
*   aplica la base correspondiente enviada por el tableSegment
*   pasandola de logica a fisica
*   @return int
*/
int transformAddress (ST tableSegment, int offset);

/**
*   Devuelve 1 o 0 (bool) acorde a si tiene el accesso permitido 
*   a dicha memoria o no (Segmentation Fault)
*   @return bool
*/ 
int isAccessAllowed (ST tableSegment, int offset) ;



#endif
