#include <stdio.h>

#include "utils.h" 

#include "vm_printer.h"
#include "vm_runner.h"

#include "instruction_printer.h"

#include "constant_printer.h"

#include "segment_table.h"

#include "virtual_machine.h"

#include "error_handler.h"

void virtualMachinePrintCodeSegment(VirtualMachine* vm);
void virtualMachinePrintConstantSegment(VirtualMachine* vm);

void virtualMachinePrint(VirtualMachine* vm) {
    virtualMachinePrintConstantSegment(vm);
    virtualMachinePrintCodeSegment(vm);
}

void virtualMachinePrintCodeSegment(VirtualMachine* vm) {
    int entryPoint = vm->registers[IP];
    vm->registers[IP] = vm->registers[CS];
    
    while (!isSegmentCodeEnded(vm)) {
        prepareInstruction(vm);
        printVirtualMachineState(vm);
        advanceInstructionPointer(vm);
    }

    vm->registers[IP] = entryPoint;
}


void virtualMachinePrintConstantSegment(VirtualMachine* vm) {
    int klogicPointer = vm->registers[KS];
    int kfisicPointer;

    while(!isSegmentConstantEnded(vm, klogicPointer)) {
        prepareConstantPointer(vm, klogicPointer, &kfisicPointer);
        printConstantItem(vm, kfisicPointer);
        advanceConstantPointer(vm, &klogicPointer, kfisicPointer);
    }
}
