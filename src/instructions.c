#include "instructions.h"

#include "data_access.h"

#include "virtual_machine.h"

#include "error_handler.h"

#include "utils.h"

#include "vm_state_handler.h"

#include <stdlib.h>
#include <time.h>

void initializeInstructions(VirtualMachine* vm) {
    // vm->instructions[0x00] = SYS;
    vm->instructions[0x01] = JMP;
    vm->instructions[0x02] = JZ;
    vm->instructions[0x03] = JP;
    vm->instructions[0x04] = JN;
    vm->instructions[0x05] = JNZ;
    vm->instructions[0x06] = JNP;
    vm->instructions[0x07] = JNN;
    // vm->instructions[0x08] = NOT;
    vm->instructions[0x09] = NULL;
    vm->instructions[0x0A] = NULL;
    vm->instructions[0x0B] = NULL;
    vm->instructions[0x0C] = NULL;
    vm->instructions[0x0D] = NULL;
    vm->instructions[0x0E] = NULL;
    // vm->instructions[0x0F] = STOP;
    // vm->instructions[0x10] = MOV;
    // vm->instructions[0x11] = ADD;
    // vm->instructions[0x12] = SUB;
    // vm->instructions[0x13] = MUL;
    // vm->instructions[0x14] = DIV;
    // vm->instructions[0x15] = CMP;
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

void SHL (VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 << dataOp2, bytes);
    updateCCRegisterHandler(vm, dataOp1 << dataOp2);
}

void SHR (VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    int sar = dataOp1 >> dataOp2;
    int signBit = 0xFFFFFFFF;
    if (dataOp2 > 0) {
        signBit = 0x80000000 >> (32 - bytes*8);
        signBit = ~(signBit >> (dataOp2 - 1));
    }

    setData(vm, vm->registers[OP1], signBit & sar, bytes);
    updateCCRegisterHandler(vm, signBit & sar);
}

void SAR (VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 >> dataOp2, bytes);
    updateCCRegisterHandler(vm, dataOp1 >> dataOp2);
}

void AND (VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 & dataOp2, bytes);
    updateCCRegisterHandler(vm, dataOp1 & dataOp2);
}

void OR (VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 | dataOp2, bytes);
    updateCCRegisterHandler(vm, dataOp1 | dataOp2);
}

void XOR (VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 ^ dataOp2, bytes);
    updateCCRegisterHandler(vm, dataOp1 ^ dataOp2);
}

void SWAP (VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp2, bytes);
    setData(vm, vm->registers[OP2], dataOp1, bytes);
}

void LDL(VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    int loadedLow = (dataOp1 & 0xFFFF0000) | (dataOp2 & 0x0000FFFF);
    setData(vm, vm->registers[OP1], loadedLow, bytes);
}

void LDH(VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    int loadedHigh = (dataOp1 & 0x0000FFFF) | (dataOp2 & 0xFFFF0000);
    setData(vm, vm->registers[OP1], loadedHigh, bytes);
}

void RND(VirtualMachine* vm) {
    int bytes = 4;

    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    srand(time(NULL));
    int randomValue = rand() % (dataOp2 + 1);

    setData(vm, vm->registers[OP1], randomValue, bytes);
    updateCCRegisterHandler(vm, randomValue);
}

void JMP(VirtualMachine* virtualM) {
    int bytes = 4;
    int data = getData(virtualM, virtualM->registers[OP1], bytes);
    
    virtualM->registers[IP] = data;    
}

void JZ(VirtualMachine* virtualM) {
    int bytes = 4;
    int data = getData(virtualM, virtualM->registers[OP1], bytes);

    unsigned int cc = virtualM->registers[CC]; 
    if (cc >> 30 == 1)
        virtualM->registers[IP] = data;
    else 
        virtualM->registers[IP]++;
}

void JP(VirtualMachine* virtualM) {
    int bytes = 4;
    int data = getData(virtualM, virtualM->registers[OP1], bytes);

    unsigned int cc = virtualM->registers[CC];
    if (cc >> 30 == 0) 
        virtualM->registers[IP] = data;
    else 
        virtualM->registers[IP]++;
}

void JN(VirtualMachine* virtualM) {
    int bytes = 4;
    int data = getData(virtualM, virtualM->registers[OP1], bytes);

    unsigned int cc = virtualM->registers[CC];
    if (cc >> 30 == 2)
        virtualM->registers[IP] = data;
    else 
        virtualM->registers[IP]++;
}

void JNZ(VirtualMachine* virtualM) {
    int bytes = 4;
    int data = getData(virtualM, virtualM->registers[OP1], bytes);

    unsigned int cc = virtualM->registers[CC];
    if (cc >> 30 == 0 || cc >> 30 == 2)
        virtualM->registers[IP] = data;
    else 
        virtualM->registers[IP]++;
}

void JNP(VirtualMachine* virtualM) {
    int bytes = 4;
    int data = getData(virtualM, virtualM->registers[OP1], bytes);

    unsigned int cc = virtualM->registers[CC];
    if (cc >> 30 == 1 || cc >> 30 == 2)
        virtualM->registers[IP] = data;
    else 
        virtualM->registers[IP]++;
}

void JNN(VirtualMachine* virtualM) {
    int bytes = 4;
    int data = getData(virtualM, virtualM->registers[OP1], bytes);

    unsigned int cc = virtualM->registers[CC];
    if (cc >> 30 == 0 || cc >> 30 == 1)
        virtualM->registers[IP] = data;
    else 
        virtualM->registers[IP]++;
}