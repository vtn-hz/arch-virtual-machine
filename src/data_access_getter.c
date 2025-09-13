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
        error_handler.invalidInstruction("instrucción inválida: tipo de operando %d", operandType);   
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
    DST segment_table = virtualM->segment_table; 
    
    int baseRegister = extractOperationBaseRegister( operand );
    int memoryOffset = extractOperationValue( operand );

    int logicMemoryAccess = virtualM->registers[ baseRegister ] + memoryOffset;
    int fisicMemoryAccess = transformLogicalAddress(segment_table, logicMemoryAccess);

    if (!isLogicalAddressValid(segment_table, logicMemoryAccess + (bytes - 1))) 
        error_handler.segmentationFault("logical address 0x%08X", logicMemoryAccess + (bytes - 1));
    
    prepareMemoryAccessHandler(virtualM, logicMemoryAccess, fisicMemoryAccess, bytes);

    int readedData = 0 ;
    for  (int i=0 ; i<bytes ; i++) {
        readedData = (readedData << 8) | virtualM->memory[ fisicMemoryAccess +  i ];        
    }
    readedData = spreadSign( readedData, 32-bytes*8 );
    commitMemoryAccessHandler( virtualM, readedData );

    return readedData;
}

int getDataFromEmpty (VirtualMachine *virtualM, int operand, int bytes) {
    error_handler.invalidInstruction("no se puede obtener dato de operando vacío");
    return 0;
}

void initializeGetters () {
    availableDataGetter[0] = getDataFromEmpty;
    availableDataGetter[1] = getDataFromRegister;
    availableDataGetter[2] = getDataFromInmediato;
    availableDataGetter[3] = getDataFromMemory;
}
