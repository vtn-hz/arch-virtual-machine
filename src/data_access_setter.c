#include <stdlib.h>

#include "data_access.h"

#include "virtual_machine.h"

#include "utils.h"

#include "error_handler.h"

#include "vm_state_handler.h"

static p_setter_data availableDataSetter[4];

void setData(VirtualMachine *virtualM, int operand, int value, int bytes) {
    int operandType = extractOperationType( operand );

    if (!(0 <= operandType && operandType <= 3)) {
        error_handler.invalidOperand(operand);   
    }
    
    availableDataSetter[ operandType ](virtualM, operand, value, bytes);
}   


void setDataToRegister(VirtualMachine *virtualM, int operand, int value, int bytes) {
    virtualM->registers[ extractOperationValue(operand) ] = value;
}

void setDataToInmediato (VirtualMachine *virtualM, int operand, int value, int bytes) {
    error_handler.buildError("Error: no se puede asignar a un inmediato");
}

void setDataToMemory(VirtualMachine *virtualM, int operand, int value, int bytes) {
    DST segment_table = virtualM->segment_table; 
    
    int baseRegister = extractOperationBaseRegister( operand );
    int memoryOffset = extractOperationValue( operand );

    int logicMemoryAccess = virtualM->registers[ baseRegister ] + memoryOffset;
    int fisicMemoryAccess = transformLogicalAddress(segment_table, logicMemoryAccess);

    if (!isLogicalAddressValid(segment_table, logicMemoryAccess + (bytes - 1))) 
        error_handler.segmentationFault(logicMemoryAccess);
    
    prepareMemoryAccessHandler( virtualM, logicMemoryAccess, fisicMemoryAccess, bytes);

    int buffer = value;
    for  (int i = bytes-1 ; i >= 0 ; i--) {
        virtualM->memory[ fisicMemoryAccess + i ] = buffer & 0xFF ;
        buffer = buffer >> 8;        
    }

    commitMemoryAccessHandler( virtualM, value );
}

void setDataToEmpty (VirtualMachine *virtualM, int operand, int value, int bytes) {
    error_handler.emptyOperand();
}

void initializeSetters () {
    availableDataSetter[0] = setDataToEmpty;
    availableDataSetter[1] = setDataToRegister;
    availableDataSetter[2] = setDataToInmediato;
    availableDataSetter[3] = setDataToMemory;
}
