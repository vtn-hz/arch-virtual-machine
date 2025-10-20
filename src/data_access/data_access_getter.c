#include <stdlib.h>

#include "data_access.h"

#include "virtual_machine.h"

#include "utils.h"

#include "error_handler.h"

#include "vm_state_handler.h"

static p_getter_data availableDataGetter[4];

int solveRegisterSectorGet(int registerValue, int sector);

int getData(VirtualMachine* vm, int operand) {
    int operandType = extractOperationType(operand);

    if (!(0 <= operandType && operandType <= 3))
        error_handler.invalidOperand(operand);   
    
    return availableDataGetter[operandType](vm, operand);
}


int getDataFromRegister(VirtualMachine* vm, int operand) {
    int registerId = extractRegisterId(operand);
    int registerValue = vm->registers[ registerId ];
    int sector = extractOperationSector(operand);

    return solveRegisterSectorGet(registerValue, sector);
}

int solveRegisterSectorGet(int registerValue, int sector) {
    int calculatedData;
    
    switch (sector) {
        case 0b00: 
            calculatedData = registerValue; 
            break;
        case 0b01: 
            calculatedData = spreadSign( applyMask(registerValue, 0x00FF, 0) , 24);
            break;
        case 0b10: 
            calculatedData = spreadSign( applyMask(registerValue, 0xFF00, 8) , 24);
            break;
        case 0b11: 
            calculatedData = spreadSign( applyMask(registerValue, 0xFFFF, 0) , 16);
            break;
          
        default:
            error_handler.buildError("Error: invalid sector in register data getter");
        
    }

    return calculatedData;
}

int getDataFromInmediato(VirtualMachine* vm, int operand) {
    return extractOperationValue(operand);
}

int getDataFromMemory(VirtualMachine* vm, int operand) {
    int baseRegister = extractOperationBaseRegister(operand);
    int memoryOffset = extractOperationValue(operand);

    int bytes = DEFAULT_ACCESS_SIZE - extractOperationCellSize(operand);
    
    prepareGetMemoryAccess(vm, baseRegister, memoryOffset, bytes);
    return commitGetMemoryAccess(vm);
}

void prepareGetMemoryAccess(VirtualMachine* vm, int baseRegister, int memoryOffset, int bytes) {
    prepareMemoryAccessHandler(vm, baseRegister, memoryOffset, bytes);
}

int commitGetMemoryAccess(VirtualMachine* vm) {
    int fisicMemoryAccess = vm->registers[MAR] & 0xFFFF;
    int bytes = (vm->registers[MAR] >> 16) & 0x0000FFFF;

    int readedData = 0 ;
    for  (int i=0 ; i<bytes ; i++) {
        readedData = (readedData << 8) | vm->memory[fisicMemoryAccess +  i];        
    }
    readedData = spreadSign(readedData, 32-bytes*8);
    
    prepareMBRHandler(vm, readedData);
    return vm->registers[MBR];
}

int getDataFromEmpty(VirtualMachine* vm, int operand) {
    error_handler.emptyOperand();
    return 0;
}

void initializeGetters() {
    availableDataGetter[0] = getDataFromEmpty;
    availableDataGetter[1] = getDataFromRegister;
    availableDataGetter[2] = getDataFromInmediato;
    availableDataGetter[3] = getDataFromMemory;
}
