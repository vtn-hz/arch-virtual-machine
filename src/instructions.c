#include "instructions.h"

#include "data_access.h"

#include "virtual_machine.h"

#include "error_handler.h"

#include "utils.h"

#include <stdlib.h>
#include <time.h>


void SHL (VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 << dataOp2, bytes);
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
}

void SAR (VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 >> dataOp2, bytes);
}

void AND (VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 & dataOp2, bytes);
}

void OR (VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 | dataOp2, bytes);
}

void XOR (VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 ^ dataOp2, bytes);
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
}
