#include <stdlib.h>

#include "data_access.h"
#include "virtual_machine.h"
#include "error_handler.h"

void initializeSetters();
void initializeGetters(void);

typedef int(*p_getter_data)(VirtualMachine*, int operand, int bytes);
typedef void(*p_setter_data)(VirtualMachine*, int operand, int value, int bytes);

static p_getter_data availableDataGetter[4];
// static p_setter_data availableDataSetter[4];

// utils/...
int extractOperationType ( int operand ) {
    return operand >> 24;
}

int extractOperationValue ( int operand ) {
    int masked = operand & 0xFFFF;
    masked = (masked << 4) >> 4; // keep sign
    return masked;
}

int extractOperationBaseRegister ( int operand ) {
    int masked = operand & 0x00FF0000;
    return masked >> 16;
}

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

    int readedData = 0 ;
    for  (int i=0 ; i<bytes ; i++) {
        readedData = (readedData << 8) | virtualM->mem[ fisicMemoryAccess +  i ];        
    }

    return (readedData << (32-bytes*8)) >> (32-bytes*8);
}

void initializeGetters () {
    availableDataGetter[0] = NULL;
    availableDataGetter[1] = getDataFromRegister;
    availableDataGetter[2] = getDataFromInmediato;
    availableDataGetter[3] = getDataFromMemory;
}

// testing main 
/*

int main(int argc, char** argv) {
    VirtualMachine* virtual = createVm ( 20 );
    vmSetUp( virtual );

    // 1111 1111 1111 1100 -> -4
    virtual->mem[20] = 0xFF;
    virtual->mem[21] = 0xFF;
    virtual->mem[22] = 0x00;


    int operand = 0x031B0000;
    int value = getData(virtual, operand, 3);
    printf("\n\nvalue: %d\n", value);
    releaseVm( virtual );
    return 0;
}
*/