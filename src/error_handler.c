#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <string.h>

#include "error_handler.h"


static char* addErrorContext(char *buffer, const char *message, const char *context) {
    if (context != NO_ERROR_CONTEXT)
        sprintf(buffer, "%s\n%s", message, context);
    else 
        sprintf(buffer, "%s", message);

    return buffer;
}


void fileNotFound(const char* context, ...) { 
    const char *message = "Error: no se encontró el archivo";
    char buffer[256];

    va_list args;
    va_start(args, context);

    vfprintf(stderr, addErrorContext(buffer, message, context), args);
    fprintf(stderr, "\n");

    va_end(args);
    exit(EXIT_FAILURE);
}

void invalidHeader(const char* context, ...) { 
    const char *message = "Error: cabecera de archivo inválida";
    char buffer[256];

    va_list args;
    va_start(args, context);

    vfprintf(stderr, addErrorContext(buffer, message, context), args);
    fprintf(stderr, "\n");

    va_end(args);
    exit(EXIT_FAILURE);
}

void invalidInstruction(const char* context, ...) { 
    const char *message = "Error: instrucción inválida";
    char buffer[256];

    va_list args;
    va_start(args, context);

    vfprintf(stderr, addErrorContext(buffer, message, context), args);
    fprintf(stderr, "\n");

    va_end(args);
    exit(EXIT_FAILURE);
}

void segmentationFault(const char* context, ...) { 
    const char *message = "Error: falla de segmentación";
    char buffer[256];

    va_list args;
    va_start(args, context);

    vfprintf(stderr, addErrorContext(buffer, message, context), args);
    fprintf(stderr, "\n");

    va_end(args);
    exit(EXIT_FAILURE);
}

void divisionByZero(const char* context, ...) { 
    const char *message = "Error: división por cero";
    char buffer[256];

    va_list args;
    va_start(args, context);

    vfprintf(stderr, addErrorContext(buffer, message, context), args);
    fprintf(stderr, "\n");

    va_end(args);
    exit(EXIT_FAILURE);
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

    customError
};
