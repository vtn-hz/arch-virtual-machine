#ifndef VM_MODE_H
#define VM_MODE_H

typedef struct VirtualMachine VirtualMachine;

#include "arguments_parsing.h"

#define DEBUG_MODE 'd'

#define GO_MODE 'g'

#define QUIT_MODE 'q'

char askDebugAction () ;

int isDebugEnabled( arguments ); 

int isDebugMode( VirtualMachine*, arguments );

void solveDebugAction( VirtualMachine*, arguments ) ; 


#endif
