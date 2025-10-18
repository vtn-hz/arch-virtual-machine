#include "utils.h"

int applyMask(int number, int mask, int shiftright_slots) {
    return (number & mask) >> shiftright_slots;
}

int spreadSign(int number, int spreadSize) {
    return (number << spreadSize) >> spreadSize;
}

int extractOperationType(int operand) {
    return operand >> 24;
}

int extractOperationValue(int operand) {
    operand = applyMask(operand, 0x0000FFFF, 0);
    return spreadSign(operand, 16);
}

int extractOperationBaseRegister(int operand) {
    return applyMask(operand, 0x00FF0000, 16);
}

char* intToString(int number){
    char* cad = ( char*) malloc (4 * sizeof(char));

    cad[0] = ((number >> 24) & 0xFF) + '0';
    cad[1] = ((number >> 16) & 0xFF) + '0';
    cad[2] = ((number >> 8) & 0xFF) + '0';
    cad[3] = (number & 0xFF) + '0';    
    
    return cad;
}
