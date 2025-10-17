#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arguments_parsing.h"
#include "error_handler.h"

void dispatchArguments(char** argv, int i, int argc, arguments* args, int sizes[]);
int solveArgumentType(char* argument, int i);

void getArguments(int argc, char** argv, arguments* args, int sizes[]) {
    int i = 0;    
    
    args->currentVmx = NULL;
    args->currentVmi = NULL;
    args->memory_size = 16; // default 16KB
    args->dissasembler = 0;
    args->paramsAmount = 0;

    // check if input has required arguments
    if (argc < 2)
        error_handler.fileNotFound();

    while ( i < argc && (strcmp(argv[i], "-p") != 0) ) {
        dispatchArguments(argv, ++i, argc, args, sizes);
    }    

    if(args->currentVmx == NULL && args->currentVmi== NULL)
        error_handler.buildError("Error: se requiere un archivo .vmx o .vmi");
}

int solveArgumentType(char* argument, int i) {
    char* argsType[5] = {
        ".vmx", // 1 vmx file
        ".vmi",  // 2 vmi file
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

void dispatchArguments(char** argv, int i, int argc, arguments* args, int sizes[]) {
    int argumentType = solveArgumentType(argv[i], i - 1);

    switch (argumentType)
    {
        case 1: // vmx
            args->currentVmx = argv[i];
            break;
        case 2: // vmi
            args->currentVmi = argv[i];
            break;
        case 3: // memory size
            args->memory_size = atoi(argv[i] + 2); // skip "m="
            break;
        case 4: // dissasembler
            args->dissasembler = 1;
            break;
        case 5: // params
            args->paramsAmount = 0;
            for (int j = i + 1; j < argc; j++) {
                args->params[args->paramsAmount] = argv[j];
                args->paramsAmount++;
                sizes[0]+= strlen(argv[j]) + 1; // +1 for null terminator
            }
            sizes[0]+=args->paramsAmount*4; // for pointers
            break;  
        default:
            error_handler.buildError("Error: argumento desconocido {%s}", argv[i]);
            break;
    }
}
