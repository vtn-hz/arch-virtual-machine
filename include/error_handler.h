
#ifndef __ERROR_HANDLER__ 
#define __ERROR_HANDLER__

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


#endif 