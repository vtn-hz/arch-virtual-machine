#include <stdlib.h>

#include "data_access.h"

#include "virtual_machine.h"

#include "utils.h"

#include "error_handler.h"

#include "vm_state_handler.h"

static p_setter_data availableDataSetter[4];

void solveRegisterSectorSet(int *_register, int sector, int value);

void setData(VirtualMachine* vm, int operand, int value) {
    int operandType = extractOperationType(operand);

    if (!(0 <= operandType && operandType <= 3))
        error_handler.invalidOperand(operand);   
    
    availableDataSetter[operandType](vm, operand, value);
}   

void setDataToRegister(VirtualMachine* vm, int operand, int value) {
    int registerId = extractRegisterId(operand);
    int sector = extractOperationSector(operand);

    solveRegisterSectorSet(vm->registers + registerId, sector, value);
}

void solveRegisterSectorSet(int *_register, int sector, int value) {
    int originalRegisterValue = *_register;
    
    switch (sector) {
        case 0b00: 
            originalRegisterValue = 0; 
            break;
        case 0b01: 
            originalRegisterValue &= 0xFFFFFF00;
            value = applyMask(value, 0x000000FF, 0);
            break;
        case 0b10: 
            originalRegisterValue &= 0xFFFF00FF;
            value = applyMask(value, 0x0000FF, -8);
            break;
        case 0b11: 
            originalRegisterValue &= 0xFFFF0000;
            value = applyMask(value, 0x0000FFFF, 0);
            break;
        default:
            error_handler.buildError("Error: invalid sector in register data getter");
        
    }

    *_register = originalRegisterValue | value;
}

void setDataToInmediato(VirtualMachine* vm, int operand, int value) {
    error_handler.buildError("Error: no se puede asignar a un inmediato");
}

void setDataToMemory(VirtualMachine* vm, int operand, int value) {
    int baseRegister = extractOperationBaseRegister(operand);
    int memoryOffset = extractOperationValue(operand);

    int bytes = DEFAULT_ACCESS_SIZE - extractOperationCellSize(operand);

    prepareSetMemoryAccess(vm, baseRegister, memoryOffset, value, bytes);
    commitSetMemoryAccess(vm);
}

void prepareSetMemoryAccess(VirtualMachine* vm, int baseRegister, int memoryOffset, int value, int bytes) {
    prepareMemoryAccessHandler(vm, baseRegister, memoryOffset, bytes);
    prepareMBRHandler(vm, value);
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

void executeDataPush(VirtualMachine *vm, int data) {
    vm->registers[SP] -= 4; 

    if (!isLogicalAddressValid(vm->segment_table, vm->registers[SP])) 
        error_handler.stackOverflow();

    prepareSetMemoryAccess(vm, SP, 0, data, DEFAULT_ACCESS_SIZE);
    commitSetMemoryAccess(vm);    
}

void setDataToEmpty(VirtualMachine* vm, int operand, int value) {
    error_handler.emptyOperand();
}

void initializeSetters() {
    availableDataSetter[0] = setDataToEmpty;
    availableDataSetter[1] = setDataToRegister;
    availableDataSetter[2] = setDataToInmediato;
    availableDataSetter[3] = setDataToMemory;
}
