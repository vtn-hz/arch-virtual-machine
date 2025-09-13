#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <string.h>

#include "error_handler.h"

void fileNotFound()         { customError("Error: no se encontró el archivo");     }
void invalidHeader()        { customError("Error: cabecera de archivo inválida");  } 
void invalidInstruction()   { customError("Error: instrucción inválida");          }   
void segmentationFault()    { customError("Error: falla de segmentación");         }
void divisionByZero()       { customError("Error: división por cero");             }

void contextedError(p_default_error_handler handler, const char* context, ...) {
    va_list args;
    va_start(args, context);

    fprintf(stderr, "<");
    vfprintf(stderr, context, args);
    fprintf(stderr, "> ");

    va_end(args);
    handler();
}

void customError(const char* context, ...) {
    va_list args;
    va_start(args, context);
    vfprintf(stderr, context, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(EXIT_FAILURE);
}

ErrorHandler error_handler = {
    fileNotFound, 
    invalidHeader, 
    invalidInstruction,
    segmentationFault, 
    divisionByZero,

    customError,
    contextedError
};
