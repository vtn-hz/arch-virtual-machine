#include <stdlib.h>

#include "utils.h"

int applyMask(int number, int mask, int shift_slots) {
    int result = number & mask;

    if (shift_slots > 0) 
        result = result >> shift_slots;
    else
        result = result << -shift_slots;
    

    return result;
}

int applyMaskAfter(int number, int mask, int shiftright_slots) {
    return (number >> shiftright_slots) & mask;
}

int spreadSign(int number, int spreadSize) {
    return (number << spreadSize) >> spreadSize;
}

int extractOperationSector(int operand) {
    return applyMaskAfter(operand, 0x3, 6);
}

int extractRegisterId(int operand) {
    return applyMask(operand, 0x1F, 0);
}

int extractOperationCellSize(int operand) {
    return applyMaskAfter(operand, 0b11, 22);
}

int extractOperationType(int operand) {
    return operand >> 24;
}

int extractOperationValue(int operand) {
    operand = applyMask(operand, 0x0000FFFF, 0);
    return spreadSign(operand, 16);
}

int extractOperationBaseRegister(int operand) {
    return applyMask(operand, 0x001F0000, 16);
}

char* toBigEndian(char* bytes, unsigned int val, int n) {
    for (int i = n-1; i >= 0; i--) {
        bytes[i] = val & 0xFF;
        val = val >> 8;  
    }

    return bytes;
}
