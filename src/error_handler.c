#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "error_handler.h"


void fileNotFound() {
    buildError("Error: no se encontró el archivo");
}

void invalidHeader() {
    buildError("Error: cabecera de archivo inválida");
} 

void invalidInstruction( int instruction ) { 
    buildError("Error: {%d} instrucción inválida", instruction);
}

void invalidOperand( int operador ) {
    buildError("Error: {%d} operador inválido", operador);
}

void emptyOperand() {
    buildError("Error: operador vacío");
}

void segmentationFault( int logical_address ) {
    buildError("Error: {%x} falla de segmentación", logical_address);
}

void divisionByZero( int a, int b ) {
    buildError("Error: {%d}/{%d} división por cero", a, b);
}

void buildError(const char* context, ...) {
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
    invalidOperand,
    emptyOperand,
    
    segmentationFault, 
    divisionByZero,

    buildError
};
