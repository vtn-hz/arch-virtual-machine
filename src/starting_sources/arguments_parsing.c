#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arguments_parsing.h"
#include "error_handler.h"


void getArguments(int argc, char** argv, arguments* args) {
    args->currentVmx = NULL;
    args->currentVmi = NULL;
    args->memory_size = 16; // default 16KB
    args->dissasembler = 0;
    args->paramsAmount = 0;

    int i;    
    // check if input has required arguments
    if (argc < 2)
        error_handler.fileNotFound();
    
    for(i = 1; i < argc; i++) {
        dispatchArguments(argv, i, argc, args);
    }

    if(args->currentVmx == NULL && args->currentVmi== NULL)
        error_handler.buildError("Error: se requiere un archivo .vmx o .vmi");
}

int solveArgumentType(char* argument, int i) {
    char** argsType = {
        ".vmx", // 1 vmx file
        ".vmi"  // 2 vmi file
        "m=",   // 3 memory size
        "-d",   // 4 dissasembler
        "-p",   // 5 params
    };

    int solvedArgumentType = -1;
    char* extPos = strrchr(argument, '.');
    char* compareTo = extPos;

    if (compareTo == NULL) 
        compareTo = strncmp(argument, "m=", 2) == 0 ? "m=" : argument;

    while ( i < 5 && solvedArgumentType == -1 )
    {
        if (strcmp(compareTo, argsType[i]) == 0)
            solvedArgumentType = i + 1;

        ++i;
    }
    
    return solvedArgumentType;
}

void dispatchArguments(char** argv, int i, int argc, arguments* args) {
    int argumentType = solveArgumentType(argv[i], i-1);

    switch (argumentType)
    {
        case 1: // vmx
            args->currentVmx = argv[i + 1];
            break;
        case 2: // vmi
            args->currentVmi = argv[i + 1];
            break;
        case 3: // memory size
            args->memory_size = atoi(argv[i + 1]+2); // skip "m="
            break;
        case 4: // dissasembler
            args->dissasembler = 1;
            break;
        case 5: // params
            args->paramsAmount = 0;
            for (int j = i + 1; j < argc; j++) {
                args->params[args->paramsAmount] = argv[j];
                args->paramsAmount++;
            }
            break;  
        default:
            error_handler.buildError("Error: argumento desconocido {%s}", argv[i]);
            break;
    }
}
