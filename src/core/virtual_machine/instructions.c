#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "instructions.h"

#include "data_access.h"

#include "virtual_machine.h"

#include "vm_state_handler.h"

#include "vm_mode.h"

#include "error_handler.h"

#include "utils.h"

#include "system_calls.h"

void initializeInstructions(VirtualMachine* vm) {
    vm->instructions[0x00] = SYS;
    vm->instructions[0x01] = JMP;
    vm->instructions[0x02] = JZ;
    vm->instructions[0x03] = JP;
    vm->instructions[0x04] = JN;
    vm->instructions[0x05] = JNZ;
    vm->instructions[0x06] = JNP;
    vm->instructions[0x07] = JNN;
    vm->instructions[0x08] = NOT;
    vm->instructions[0x09] = NULL;
    vm->instructions[0x0A] = NULL;
    vm->instructions[0x0B] = PUSH;
    vm->instructions[0x0C] = POP;
    vm->instructions[0x0D] = CALL;
    vm->instructions[0x0E] = RET;
    vm->instructions[0x0F] = STOP;
    vm->instructions[0x10] = MOV;
    vm->instructions[0x11] = ADD;
    vm->instructions[0x12] = SUB;
    vm->instructions[0x13] = MUL;
    vm->instructions[0x14] = DIV;
    vm->instructions[0x15] = CMP;
    vm->instructions[0x16] = SHL;
    vm->instructions[0x17] = SHR;
    vm->instructions[0x18] = SAR;
    vm->instructions[0x19] = AND;
    vm->instructions[0x1A] = OR;
    vm->instructions[0x1B] = XOR;
    vm->instructions[0x1C] = SWAP;
    vm->instructions[0x1D] = LDL;
    vm->instructions[0x1E] = LDH;
    vm->instructions[0x1F] = RND;
}

void MOV(VirtualMachine* vm) {
    int data = getData(vm, vm->registers[OP2]);
    setData(vm, vm->registers[OP1], data);
}

void ADD(VirtualMachine* vm) {
    int data1 = getData(vm, vm->registers[OP1]);
    int data2 = getData(vm, vm->registers[OP2]);

    setData(vm, vm->registers[OP1], data1 + data2);
    updateCCRegisterHandler(vm, data1 + data2);
}

void SUB(VirtualMachine* vm) {
    int data1 = getData(vm, vm->registers[OP1]);
    int data2 = getData(vm, vm->registers[OP2]);

    setData(vm, vm->registers[OP1], data1 - data2);
    updateCCRegisterHandler(vm, data1 - data2);
}

void MUL(VirtualMachine* vm) {
    int data1 = getData(vm, vm->registers[OP1]);
    int data2 = getData(vm, vm->registers[OP2]);

    setData(vm, vm->registers[OP1], data1 * data2);
    updateCCRegisterHandler(vm, data1 * data2);
}

void DIV(VirtualMachine* vm) {
    int data1 = getData(vm, vm->registers[OP1]);
    int data2 = getData(vm, vm->registers[OP2]);
    if (data2 == 0)
        error_handler.divisionByZero(data1, data2);

    setData(vm, vm->registers[OP1], data1 / data2);
    updateCCRegisterHandler(vm, data1 / data2);
    vm->registers[AC] = data1 % data2; 
}

void CMP(VirtualMachine* vm) {
    int data1 = getData(vm, vm->registers[OP1]);
    int data2 = getData(vm, vm->registers[OP2]);

    updateCCRegisterHandler(vm, data1 - data2);
}

void SHL(VirtualMachine* vm) {
    int dataOp1 = getData(vm, vm->registers[OP1]);
    int dataOp2 = getData(vm, vm->registers[OP2]);

    setData(vm, vm->registers[OP1], dataOp1 << dataOp2);
    updateCCRegisterHandler(vm, dataOp1 << dataOp2);
}

void SHR(VirtualMachine* vm) {
    int dataOp1 = getData(vm, vm->registers[OP1]);
    int dataOp2 = getData(vm, vm->registers[OP2]);

    int sar = dataOp1 >> dataOp2;
    int signBit = 0xFFFFFFFF;
    if (dataOp2 > 0) {
        signBit = 0x80000000 ; 
        /**
         * que deberia hacer ahora que sabemos como funcionan la memoria en pedacitos?
         * deberiamos hacer a partir de lo que cree que leyo el usuario de la maquina virtual?
         * si es asi, exactamnete desde donde deberiamos comenzar a leer? 
         * porque si era un registro op2 se hace una cosa o sino otra?
         * 
         * antes: signBit = 0x80000000 >> (32 - bytes*8);
         */
        signBit = ~(signBit >> (dataOp2 - 1));
    }

    setData(vm, vm->registers[OP1], signBit & sar);
    updateCCRegisterHandler(vm, signBit & sar);
}

void SAR(VirtualMachine* vm) {
    int dataOp1 = getData(vm, vm->registers[OP1]);
    int dataOp2 = getData(vm, vm->registers[OP2]);

    setData(vm, vm->registers[OP1], dataOp1 >> dataOp2);
    updateCCRegisterHandler(vm, dataOp1 >> dataOp2);
}

void AND(VirtualMachine* vm) {
    int dataOp1 = getData(vm, vm->registers[OP1]);
    int dataOp2 = getData(vm, vm->registers[OP2]);

    setData(vm, vm->registers[OP1], dataOp1 & dataOp2);
    updateCCRegisterHandler(vm, dataOp1 & dataOp2);
}

void OR(VirtualMachine* vm) {
    int dataOp1 = getData(vm, vm->registers[OP1]);
    int dataOp2 = getData(vm, vm->registers[OP2]);

    setData(vm, vm->registers[OP1], dataOp1 | dataOp2);
    updateCCRegisterHandler(vm, dataOp1 | dataOp2);
}

void XOR(VirtualMachine* vm) {
    int dataOp1 = getData(vm, vm->registers[OP1]);
    int dataOp2 = getData(vm, vm->registers[OP2]);

    setData(vm, vm->registers[OP1], dataOp1 ^ dataOp2);
    updateCCRegisterHandler(vm, dataOp1 ^ dataOp2);
}

void SWAP(VirtualMachine* vm) {
    int dataOp1 = getData(vm, vm->registers[OP1]);
    int dataOp2 = getData(vm, vm->registers[OP2]);

    setData(vm, vm->registers[OP1], dataOp2);
    setData(vm, vm->registers[OP2], dataOp1);
}

void LDL(VirtualMachine* vm) {
    int dataOp1 = getData(vm, vm->registers[OP1]);
    int dataOp2 = getData(vm, vm->registers[OP2]);

    int loadedLow = (dataOp1 & 0xFFFF0000) | (dataOp2 & 0x0000FFFF);
    setData(vm, vm->registers[OP1], loadedLow);
}

void LDH(VirtualMachine* vm) {
    int dataOp1 = getData(vm, vm->registers[OP1]);
    int dataOp2 = getData(vm, vm->registers[OP2]);

    int loadedHigh = (dataOp1 & 0x0000FFFF) | ((dataOp2 & 0x0000FFFF) << 16);
    setData(vm, vm->registers[OP1], loadedHigh);
}

void RND(VirtualMachine* vm) {
    int dataOp2 = getData(vm, vm->registers[OP2]);

    srand(time(NULL));
    int randomValue = rand() % (dataOp2 + 1);

    setData(vm, vm->registers[OP1], randomValue);
    updateCCRegisterHandler(vm, randomValue);
}

void JMP(VirtualMachine* vm) {
int data = getData(vm, vm->registers[OP1]);
    
    vm->registers[IP] = vm->registers[CS] | data;
}

void JZ(VirtualMachine* vm) {
    int data = getData(vm, vm->registers[OP1]);

    unsigned int cc = vm->registers[CC]; 
    if (cc >> 30 == 1)
        vm->registers[IP] = vm->registers[CS] | data;

}

void JP(VirtualMachine* vm) {
    int data = getData(vm, vm->registers[OP1]);

    unsigned int cc = vm->registers[CC];
    if (cc >> 30 == 0) 
        vm->registers[IP] = vm->registers[CS] | data;

}

void JN(VirtualMachine* vm) {
    int data = getData(vm, vm->registers[OP1]);

    unsigned int cc = vm->registers[CC];
    if (cc >> 30 == 2)
        vm->registers[IP] = vm->registers[CS] | data;

}

void JNZ(VirtualMachine* vm) {
    int data = getData(vm, vm->registers[OP1]);

    unsigned int cc = vm->registers[CC];
    if (cc >> 30 == 0 || cc >> 30 == 2)
        vm->registers[IP] = vm->registers[CS] | data;

}

void JNP(VirtualMachine* vm) {
    int data = getData(vm, vm->registers[OP1]);

    unsigned int cc = vm->registers[CC];
    if (cc >> 30 == 1 || cc >> 30 == 2)
        vm->registers[IP] = vm->registers[CS] | data;
}

void JNN(VirtualMachine* vm) {
    int data = getData(vm, vm->registers[OP1]);

    unsigned int cc = vm->registers[CC];
    if (cc >> 30 == 0 || cc >> 30 == 1)
        vm->registers[IP] = vm->registers[CS] | data;

}

void SYS(VirtualMachine* vm) {
    dispatchSystemCall(vm);
}

void STOP(VirtualMachine* vm) {
    vm->registers[IP] = -1; 
}

void NOT(VirtualMachine* vm) {
    int data = getData(vm, vm->registers[OP1]);
    setData(vm, vm->registers[OP1], ~data);
    updateCCRegisterHandler(vm, ~data);
}

void PUSH(VirtualMachine* vm) {
    int data = getData(vm, vm->registers[OP1]);
    vm->registers[SP] -= 4; 

    if (!isLogicalAddressValid(vm->segment_table, vm->registers[SP])) 
        error_handler.stackOverflow();

    /*

        Cada vez que se realiza una operación en la memoria, se debe cargar en el registro LAR la dirección
        lógica a la que se quiere acceder y la cantidad de bytes en la parte alta del registro MAR (los 2 bytes más
        significativos). Luego de realizar la traducción a una dirección física, el resultado debe almacenarse en la
        parte baja del registro MAR (los 2 bytes menos significativos). En el registro MBR debe quedar el valor con
        el cual se está operando, ya sea el valor que se desea almacenar en el caso de una escritura o el que se
        obtuvo después de la lectura. La lectura de la instrucción no debe modificar ninguno de estos registros.

        valen:   yo interpreto que "cada vez que se realiza una operación en la memoria" 
                 significa que es cualquier acceso a memoria (STACK INCLUÍDO)
    */ 
    prepareSetMemoryAccess(vm, SP, 0, DEFAULT_ACCESS_SIZE, data);
    commitSetMemoryAccess(vm);
}

void POP(VirtualMachine* vm) {
    
    if (!isLogicalAddressValid(vm->segment_table, vm->registers[SP])) 
        error_handler.stackUnderflow();

    prepareGetMemoryAccess(vm, SP, 0, DEFAULT_ACCESS_SIZE);
    setData(vm, vm->registers[OP1], commitGetMemoryAccess(vm));
    
    vm->registers[SP] += 4;
}

void CALL(VirtualMachine* vm) {
    int data = getData(vm, vm->registers[OP1]);

    vm->registers[SP] -= 4;
    if (!isLogicalAddressValid(vm->segment_table, vm->registers[SP])) 
        error_handler.stackOverflow();

    prepareSetMemoryAccess(vm, SP, 0, DEFAULT_ACCESS_SIZE, vm->registers[IP]);
    commitSetMemoryAccess(vm);

    vm->registers[IP] = vm->registers[CS] | (data & 0xFFFF);
}

void RET(VirtualMachine* vm) {

    if (!isLogicalAddressValid(vm->segment_table, vm->registers[SP])) 
        error_handler.stackUnderflow();

    prepareGetMemoryAccess(vm, SP, 0, DEFAULT_ACCESS_SIZE);
    vm->registers[IP] = commitGetMemoryAccess(vm);

    vm->registers[SP] += 4;
}