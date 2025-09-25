#include <stdio.h>

#include "instruction_printer.h"

#include "mnemonics_str.h"
#include "registers_str.h"

#include "utils.h"

#include "virtual_machine.h"

#include "error_handler.h"

void printMnemonic(int opCode);

void printOperand(int operand, int opCode);

void printOperandRegister   (int operand);
void printOperandInmmediate (int operand, int opCode);
void printOperandMemory     (int operand); 

void printInstruction(VirtualMachine* vm) {
    int opCode = vm->registers[OPC];
    int operand1 = vm->registers[OP1];
    int operand2 = vm->registers[OP2];
    
    printMnemonic(opCode);
    
    if (extractOperationType(operand1) != 0)
        printOperand(operand1, opCode);

    if (extractOperationType(operand2) != 0) {
        printf(",");
        printOperand(operand2, opCode);
    }
}

void printMnemonic(int opCode) {
    if (opCode < 0 || opCode > 0x1F) 
        error_handler.invalidInstruction(opCode);

    if (MNEMONICS_STR[opCode] == NULL)
        error_handler.invalidInstruction(opCode);
    
    printf("%-4s", MNEMONICS_STR[opCode]);
}

void printOperand(int operand, int opCode) {
    int operandType = extractOperationType(operand);

    switch (operandType) {
        case 0x0: break;
        case 0x1: printOperandRegister  (operand); break;
        case 0x2: printOperandInmmediate(operand, opCode); break;
        case 0x3: printOperandMemory    (operand); break;

        default:
            error_handler.invalidOperand(operandType);
    }
}

void printOperandRegister(int operand) {
    int registerCode = extractOperationValue(operand);

    if (registerCode < 0 || registerCode > 31)
        error_handler.buildError("Error: {%x} es registro invalido ", registerCode);

    if (REGISTERS_STR[registerCode] == NULL)
        error_handler.buildError("Error: {%x} es registro invalido ", registerCode);

    printf("%10s", REGISTERS_STR[registerCode]);
}

void printOperandInmmediate(int operand, int opCode) {
    char buffer[64];
    int printedInmmediate = extractOperationValue(operand);
    if (0x01 <= opCode && opCode <= 0x07) 
        sprintf(buffer, "0x%X", printedInmmediate);
    else
        sprintf(buffer, "%d", printedInmmediate); 

    printf("%10s", buffer);
}

void printOperandMemory(int operand) {
    int registerCode = extractOperationBaseRegister(operand);
    int offset       = extractOperationValue(operand); 

    if (registerCode < 0 || registerCode > 31)
        error_handler.buildError("Error: {%x} registro es invalido ", registerCode);

    if (REGISTERS_STR[registerCode] == NULL)
        error_handler.buildError("Error: {%x} registro es invalido ", registerCode);

    char buffer[64];
    if (offset != 0) {
        char sign = offset > 0 ? '+' : '-';
        offset = offset > 0 ? offset : -offset;
        sprintf(buffer, "[%s %c %d]", REGISTERS_STR[registerCode], sign, offset);
    } else
        sprintf(buffer, "[%s]", REGISTERS_STR[registerCode]);

    printf("%10s", buffer);
}
