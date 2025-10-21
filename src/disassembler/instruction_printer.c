#include <stdio.h>

#include "instruction_printer.h"

#include "mnemonics_str.h"
#include "registers_str.h"

#include "utils.h"

#include "virtual_machine.h"

#include "error_handler.h"

void printMnemonic(int opCode);

void printOperand(int operand, int opCode);

void printInstruction(VirtualMachine* vm);

void rawInstructionPrint(VirtualMachine* vm, int entryPoint);

void printOperandRegister   (int operand);

void printOperandInmmediate (int operand, int opCode);

void printOperandMemory     (int operand); 

void printVirtualMachineState(VirtualMachine* vm, int entryPoint) {
    rawInstructionPrint(vm, entryPoint);
    printf(" | ");
    printInstruction(vm);
    printf("\n");
}

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
    int registerCode = extractRegisterId(operand);

    if (registerCode < 0 || registerCode > 31)
        error_handler.buildError("Error: {%x} es registro invalido ", registerCode);

    if (REGISTERS_STR[registerCode] == NULL)
        error_handler.buildError("Error: {%x} es registro invalido ", registerCode);

    char *registerStr;
    if ( registerCode >= 10 && registerCode <= 15 ) {
        int sector = extractOperationSector(operand);
        registerStr = (char*) REGISTERS_STR_SECTOR[registerCode - 10][sector];
    }else 
        registerStr = (char*) REGISTERS_STR[registerCode];
    

    printf("%10s", registerStr);
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
    int tagId        = extractOperationCellSize(operand);
    int registerCode = extractOperationBaseRegister(operand);
    int offset       = extractOperationValue(operand); 

    if (registerCode < 0 || registerCode > 31)
        error_handler.buildError("Error: {%x} registro es invalido ", registerCode);

    if (REGISTERS_STR[registerCode] == NULL)
        error_handler.buildError("Error: {%x} registro es invalido ", registerCode);

    const char* sizeTags[4] = {"", "w", 0, "b"};
    char buffer[64];

    if (offset != 0) {
        char sign = offset > 0 ? '+' : '-';
        offset = offset > 0 ? offset : -offset;
        sprintf(buffer, "%s[%s %c %d]", sizeTags[tagId], REGISTERS_STR[registerCode], sign, offset);
    } else
        sprintf(buffer, "%s[%s]", sizeTags[tagId], REGISTERS_STR[registerCode]);

    printf("%10s", buffer);
}

void rawInstructionPrint(VirtualMachine* vm, int entryPoint) {
    int marginLeft = 7;
    int opaSize = extractOperationType(vm->registers[OP1]);
    int opbSize = extractOperationType(vm->registers[OP2]);
    int opcSize = 1;

    if (opaSize < 0 || opaSize > 3) 
        error_handler.invalidOperand(opaSize);

    if (opbSize < 0 || opbSize > 3) 
        error_handler.invalidOperand(opbSize);

    int totalSize = opcSize + opaSize + opbSize;
    int fisicIp = transformLogicalAddress(vm->segment_table, vm->registers[IP]);

    if (entryPoint != (vm->registers[IP]))
        printf(" ");
    else
        printf(">");

    printf("[%04X]", fisicIp);
    for(int i=fisicIp; i < fisicIp + totalSize ; i++) 
        printf(" %02X", vm->memory[i]);
    
    int bytesRestantes = marginLeft - totalSize;
    for(int i=0; i < bytesRestantes ; i++) 
        printf("   ");    
}
