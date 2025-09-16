#include <stdlib.h>

#include "data_access.h"

#include "virtual_machine.h"

#include "utils.h"

#include "error_handler.h"

#include "vm_state_handler.h"

static p_getter_data availableDataGetter[4];

int getData(VirtualMachine *virtualM, int operand, int bytes) {
    int operandType = extractOperationType( operand );

    if (!(0 <= operandType && operandType <= 3)) {
        error_handler.invalidOperand(operand);   
    }
    
    return availableDataGetter[ operandType ](virtualM, operand, bytes);
}


int getDataFromRegister(VirtualMachine *virtualM, int operand, int bytes) {
    return virtualM->registers[ extractOperationValue(operand) ];
}

int getDataFromInmediato (VirtualMachine *virtualM, int operand, int bytes) {
    return extractOperationValue( operand );
}

// ya deberiamos comenzar a utilizar el tipo int32_t
int getDataFromMemory (VirtualMachine *virtualM, int operand, int bytes) {
    int baseRegister = extractOperationBaseRegister( operand );
    int memoryOffset = extractOperationValue( operand );
    
    prepareMemoryAccessHandler( virtualM, baseRegister, memoryOffset, bytes );
    return commitGetMemoryAccess( virtualM );
}

void prepareGetMemoryAccess(VirtualMachine *vm, int baseRegister, int memoryOffset, int bytes) {
    prepareMemoryAccessHandler( vm, baseRegister, memoryOffset, bytes );
}

int commitGetMemoryAccess(VirtualMachine *vm) {
    int fisicMemoryAccess = vm->registers[MAR] & 0xFFFF;
    int bytes = (vm->registers[MAR] >> 16) & 0x0000FFFF;

    int readedData = 0 ;
    for  (int i=0 ; i<bytes ; i++) {
        readedData = (readedData << 8) | vm->memory[ fisicMemoryAccess +  i ];        
    }
    readedData = spreadSign( readedData, 32-bytes*8 );
    
    return vm->registers[MBR] = readedData;
}

int getDataFromEmpty (VirtualMachine *virtualM, int operand, int bytes) {
    error_handler.emptyOperand();
    return 0;
}

void initializeGetters () {
    availableDataGetter[0] = getDataFromEmpty;
    availableDataGetter[1] = getDataFromRegister;
    availableDataGetter[2] = getDataFromInmediato;
    availableDataGetter[3] = getDataFromMemory;
}
