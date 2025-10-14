#include <stdio.h>
#include <stdlib.h>

#include "virtual_machine.h"

#include "vm_mode.h"

int isDebugMode( VirtualMachine* vm ) {
    return vm->mode == DEBUG_MODE;
}

char askDebugAction() {
    char c = getchar();

    if (c != 'g' && c != 'q') {
        c = 'd';
    }

    return c;
}

void solveDebugAction(VirtualMachine* vm, char* imagePath, char action) {
    vm->mode = action;

    switch(vm->mode) {
        case DEBUG_MODE: /* generate image */ break;
 
        case QUIT_MODE: exit(EXIT_SUCCESS); break;
    }
}  