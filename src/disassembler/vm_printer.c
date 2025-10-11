#include <stdio.h>

#include "utils.h" 

#include "vm_printer.h"
#include "vm_runner.h"

#include "instruction_printer.h"

#include "segment_table.h"

#include "virtual_machine.h"

#include "error_handler.h"

void printVirtualMachineState(VirtualMachine*);
void rawInstructionPrint(VirtualMachine*);

void virtualMachinePrint(VirtualMachine* vm) {
    while (!isSegmentCodeEnded(vm)) {
        prepareInstruction(vm);
        printVirtualMachineState(vm);
        advanceInstructionPointer(vm);
    }

    vm->registers[IP] = vm->registers[CS];
}

void printVirtualMachineState(VirtualMachine* vm) {
    rawInstructionPrint(vm);
    printf(" | ");
    printInstruction(vm);
    printf("\n");
}

void rawInstructionPrint(VirtualMachine* vm) {
    int marginLeft = 8;
    int opaSize = extractOperationType(vm->registers[OP1]);
    int opbSize = extractOperationType(vm->registers[OP2]);
    int opcSize = 1;

    if (opaSize < 0 || opaSize > 3) 
        error_handler.invalidOperand(opaSize);

    if (opbSize < 0 || opbSize > 3) 
        error_handler.invalidOperand(opbSize);

    int totalSize = opcSize + opaSize + opbSize;
    int fisicIp = transformLogicalAddress(vm->segment_table, vm->registers[IP]);

    printf("[%04X]", fisicIp);
    for(int i=fisicIp; i < fisicIp + totalSize ; i++) 
        printf(" %02X", vm->memory[i]);
    
    int bytesRestantes = marginLeft - totalSize;
    for(int i=0; i < bytesRestantes ; i++) 
        printf("   ");    
}
