#include <stdlib.h>

#include "data_access.h"

#include "virtual_machine.h"

#include "utils.h"

#include "error_handler.h"

static p_getter_data availableDataGetter[4];

int getData(VirtualMachine *virtualM, int operand, int bytes) {
    int operandType = extractOperationType( operand );
    if (!(0 <= operandType && operandType <= 3)) {
        error_handler.invalidInstruction();   
    }
    
    return availableDataGetter[ operandType ](virtualM, operand, bytes);
}

int getDataFromRegister(VirtualMachine *virtualM, int operand, int bytes) {
    return virtualM->reg[ extractOperationValue(operand) ];
}

int getDataFromInmediato (VirtualMachine *virtualM, int operand, int bytes) {
    return extractOperationValue( operand );
}

// ya deberiamos comenzar a utilizar el tipo int32_t
int getDataFromMemory (VirtualMachine *virtualM, int operand, int bytes) {
    int baseRegister = extractOperationBaseRegister( operand );
    int memoryOffset = extractOperationValue( operand );

    int logicMemoryAccess = virtualM->reg[ baseRegister ] + memoryOffset;
    int fisicMemoryAccess = 20 + memoryOffset; /*transformAddress(virtualM->table_seg, logicMemoryAccess );*/ // doesnt existst yet

    // call memoryUpdateHandler( EVENT::GET, ...REQUIRED PARAMS)

    int readedData = 0 ;
    for  (int i=0 ; i<bytes ; i++) {
        /**
         * CUIDADO!!! AL NO UTILIZAR transformAddress, TE ESTAS SALTANDO EL CONTROL DE SEGMENTOS 
         * */ 
        readedData = (readedData << 8) | virtualM->mem[ fisicMemoryAccess +  i ];        
    }

    return spreadSign( readedData, 32-bytes*8 );
}

int getDataFromEmpty (VirtualMachine *virtualM, int operand, int bytes) {
    error_handler.invalidInstruction();
    return 0;
}

void initializeGetters () {
    availableDataGetter[0] = getDataFromEmpty;
    availableDataGetter[1] = getDataFromRegister;
    availableDataGetter[2] = getDataFromInmediato;
    availableDataGetter[3] = getDataFromMemory;
}
