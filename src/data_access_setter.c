#include <stdlib.h>

#include "data_access.h"

#include "virtual_machine.h"

#include "utils.h"

#include "error_handler.h"

static p_setter_data availableDataSetter[4];

void setData(VirtualMachine *virtualM, int operand, int value, int bytes) {
    int operandType = extractOperationType( operand );

    if (!(0 <= operandType && operandType <= 3)) {
        error_handler.invalidInstruction();   
    }
    
    availableDataSetter[ operandType ](virtualM, operand, value, bytes);
}   


void setDataToRegister(VirtualMachine *virtualM, int operand, int value, int bytes) {
    virtualM->registers[ extractOperationValue(operand) ] = value;
}

void setDataToInmediato (VirtualMachine *virtualM, int operand, int value, int bytes) {
    error_handler.invalidInstruction();
}

void setDataToMemory(VirtualMachine *virtualM, int operand, int value, int bytes) {
    DST segment_table = virtualM->segment_table; 
    
    int baseRegister = extractOperationBaseRegister( operand );
    int memoryOffset = extractOperationValue( operand );

    int logicMemoryAccess = virtualM->registers[ baseRegister ] + memoryOffset;
    int fisicMemoryAccess = transformLogicalAddress(segment_table, logicMemoryAccess);

    if (!isLogicalAddressValid(segment_table, logicMemoryAccess + (bytes - 1))) 
        error_handler.segmentationFault();
    
    // call memoryUpdateHandler( EVENT::SET, ...REQUIRED PARAMS)

    for  (int i = bytes-1 ; i >= 0 ; i--) {
        virtualM->memory[ fisicMemoryAccess + i ] = value & 0xFF ;
        value = value >> 8;        
    }

}

void setDataToEmpty (VirtualMachine *virtualM, int operand, int value, int bytes) {
    error_handler.invalidInstruction();
}

void initializeSetters () {
    availableDataSetter[0] = setDataToEmpty;
    availableDataSetter[1] = setDataToRegister;
    availableDataSetter[2] = setDataToInmediato;
    availableDataSetter[3] = setDataToMemory;
}
