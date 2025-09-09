#include <stdio.h>
#include <stdlib.h>

#include "error_handler.h"

void fileNotFound() {
    fprintf(stderr, "%s", "No se encontró el archivo\n");
    exit(EXIT_FAILURE);
}

void invalidHeader() {
    fprintf(stderr, "%s", "Cabecera de archivo inválida\n");
    exit(EXIT_FAILURE);
} 

void invalidInstruction() {
    fprintf(stderr, "%s", "Instrucción inválida\n");
    exit(EXIT_FAILURE);
}

void segmentationFault() {
    fprintf(stderr, "%s", "Error de segmentación\n");
    exit(EXIT_FAILURE);
}

void divisionByZero() {
    fprintf(stderr, "%s", "Error de división por cero\n");
    exit(EXIT_FAILURE);
}

ErrorHandler error_handler = {
    fileNotFound, 
    invalidHeader, 
    invalidInstruction,
    segmentationFault, 
    divisionByZero
};
