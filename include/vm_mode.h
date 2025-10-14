#ifndef VM_MODE_H
#define VM_MODE_H

typedef struct VirtualMachine VirtualMachine;

#define DEBUG_MODE 'd'

#define GO_MODE 'g'

#define QUIT_MODE 'q'

char askDebugAction () ;

int isDebugMode( VirtualMachine* );

void solveDebugAction( VirtualMachine*, char*, char ) ; 


#endif
