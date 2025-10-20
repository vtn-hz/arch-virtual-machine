#include <stdio.h>
#include <stdlib.h>

#include "virtual_machine.h"

#include "vm_image.h"

#include "vm_mode.h"

int isDebugEnabled( void* params ) {
    return 1; // check params
}

int isDebugMode( VirtualMachine* vm, void* params ) {
    if ( !isDebugEnabled(params) ) 
        return 0;

    return vm->mode == DEBUG_MODE;
}

char askDebugAction() {
    char c = getchar();

    if (c != 'g' && c != 'q') {
        c = 'd';
    }

    return c;
}

void solveDebugAction(VirtualMachine* vm, arguments args, char action) {
    vm->mode = action;

    switch(vm->mode) {
        case DEBUG_MODE: buildImage(vm, args); break;
 
        case QUIT_MODE:  exit(EXIT_SUCCESS);   break;
    }
}  