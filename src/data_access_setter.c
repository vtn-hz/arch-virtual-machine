#include <stdlib.h>

#include "data_access.h"

#include "virtual_machine.h"

#include "utils.h"

#include "error_handler.h"

#include "vm_state_handler.h"

static p_setter_data availableDataSetter[4];

void setData(VirtualMachine* vm, int operand, int value, int bytes) {
    int operandType = extractOperationType(operand);

    if (!(0 <= operandType && operandType <= 3))
        error_handler.invalidOperand(operand);   
    
    availableDataSetter[operandType](vm, operand, value, bytes);
}   


void setDataToRegister(VirtualMachine* vm, int operand, int value, int bytes) {
    vm->registers[extractOperationValue(operand)] = value;
}

void setDataToInmediato(VirtualMachine* vm, int operand, int value, int bytes) {
    error_handler.buildError("Error: no se puede asignar a un inmediato");
}

void setDataToMemory(VirtualMachine* vm, int operand, int value, int bytes) {
    int baseRegister = extractOperationBaseRegister(operand);
    int memoryOffset = extractOperationValue(operand);

    prepareSetMemoryAccess(vm, baseRegister, memoryOffset, value, bytes);
    commitSetMemoryAccess(vm);
}

void prepareSetMemoryAccess(VirtualMachine* vm, int baseRegister, int memoryOffset, int value, int bytes) {
    prepareMemoryAccessHandler(vm, baseRegister, memoryOffset, bytes);
    vm->registers[MBR] = value;
}

void commitSetMemoryAccess(VirtualMachine* vm) {
    int fisicMemoryAccess = vm->registers[MAR] & 0xFFFF;
    int bytes = (vm->registers[MAR] >> 16) & 0x0000FFFF;
    int buffer = vm->registers[MBR];

    for  (int i = bytes-1 ; i >= 0 ; i--) {
        vm->memory[fisicMemoryAccess + i] = buffer & 0xFF ;
        buffer = buffer >> 8;        
    }
}

void setDataToEmpty(VirtualMachine* vm, int operand, int value, int bytes) {
    error_handler.emptyOperand();
}

void initializeSetters() {
    availableDataSetter[0] = setDataToEmpty;
    availableDataSetter[1] = setDataToRegister;
    availableDataSetter[2] = setDataToInmediato;
    availableDataSetter[3] = setDataToMemory;
}
