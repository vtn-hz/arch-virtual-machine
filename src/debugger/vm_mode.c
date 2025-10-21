#include <stdio.h>
#include <stdlib.h>

#include "virtual_machine.h"

#include "vm_image.h"

#include "vm_mode.h"

#include "arguments_parsing.h"

int isDebugEnabled( arguments args ) {
    return args.currentVmi != NULL;
}

int isDebugMode( VirtualMachine* vm, arguments args ) {
    if ( !isDebugEnabled(args) ) 
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
 
        case QUIT_MODE:  exit(EXIT_SUCCESS);    break;
    }
}  