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
    int baseRegister = extractOperationBaseRegister( operand );
    int memoryOffset = extractOperationValue( operand );

    int logicMemoryAccess = virtualM->registers[ baseRegister ] + memoryOffset;
    int fisicMemoryAccess = 20 + memoryOffset; /*transformAddress(virtualM->table_seg, logicMemoryAccess );*/ // doesnt existst yet

    /**
     * suponiendo que el usuario quiere guardar 2 bytes 
     * deberiamos tomar el valor desde el medio y guardar los 2 ultimos
     * o comenzar desde el principio y guardar los 2 primeros
     * 
     *  0x12 34 56 78
     * 
     *  la primera alternativa guardia 0x56 78
     *  la segunda 0x12 34
     * 
     *  por el momento se decidio la primera, guardaria 0x56 78
     */

    // call memoryUpdateHandler( EVENT::SET, ...REQUIRED PARAMS)

    for  (int i = bytes-1 ; i >= 0 ; i--) {
        /**
         * CUIDADO!!! AL NO UTILIZAR transformAddress, TE ESTAS SALTANDO EL CONTROL DE SEGMENTOS
         */
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
