#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "instructions.h"

#include "data_access.h"

#include "virtual_machine.h"

#include "vm_state_handler.h"

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
    vm->instructions[0x0B] = NULL;
    vm->instructions[0x0C] = NULL;
    vm->instructions[0x0D] = NULL;
    vm->instructions[0x0E] = NULL;
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
    int bytes = 4;

    int data = getData(vm, vm->registers[OP2], bytes);
    setData(vm, vm->registers[OP1], data, bytes);
}

void ADD(VirtualMachine* vm) {
    int bytes = 4;

    int data1 = getData(vm, vm->registers[OP1], bytes);
    int data2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], data1 + data2, bytes);
    updateCCRegisterHandler(vm, data1 + data2);
}

void SUB(VirtualMachine* vm) {
    int bytes = 4;

    int data1 = getData(vm, vm->registers[OP1], bytes);
    int data2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], data1 - data2, bytes);
    updateCCRegisterHandler(vm, data1 - data2);
}

void MUL(VirtualMachine* vm) {
    int bytes = 4;

    int data1 = getData(vm, vm->registers[OP1], bytes);
    int data2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], data1 * data2, bytes);
    updateCCRegisterHandler(vm, data1 * data2);
}

void DIV(VirtualMachine* vm) {
    int bytes = 4;

    int data1 = getData(vm, vm->registers[OP1], bytes);
    int data2 = getData(vm, vm->registers[OP2], bytes);
    if (data2 == 0)
        error_handler.divisionByZero(data1, data2);

    setData(vm, vm->registers[OP1], data1 / data2, bytes);
    updateCCRegisterHandler(vm, data1 / data2);
    vm->registers[AC] = data1 % data2; 
}

void CMP(VirtualMachine* vm) {
    int bytes = 4;

    int data1 = getData(vm, vm->registers[OP1], bytes);
    int data2 = getData(vm, vm->registers[OP2], bytes);

    updateCCRegisterHandler(vm, data1 - data2);
}

void SHL(VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 << dataOp2, bytes);
    updateCCRegisterHandler(vm, dataOp1 << dataOp2);
}

void SHR(VirtualMachine* vm) {
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

void SAR(VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 >> dataOp2, bytes);
    updateCCRegisterHandler(vm, dataOp1 >> dataOp2);
}

void AND(VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 & dataOp2, bytes);
    updateCCRegisterHandler(vm, dataOp1 & dataOp2);
}

void OR(VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 | dataOp2, bytes);
    updateCCRegisterHandler(vm, dataOp1 | dataOp2);
}

void XOR(VirtualMachine* vm) {
    int bytes = 4;

    int dataOp1 = getData(vm, vm->registers[OP1], bytes);
    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    setData(vm, vm->registers[OP1], dataOp1 ^ dataOp2, bytes);
    updateCCRegisterHandler(vm, dataOp1 ^ dataOp2);
}

void SWAP(VirtualMachine* vm) {
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

    int loadedHigh = (dataOp1 & 0x0000FFFF) | ((dataOp2 & 0x0000FFFF) << 16);
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

void JMP(VirtualMachine* vm) {
    int bytes = 4;
    int data = getData(vm, vm->registers[OP1], bytes);
    
    vm->registers[IP] = vm->registers[CS] | data;
}

void JZ(VirtualMachine* vm) {
    int bytes = 4;
    int data = getData(vm, vm->registers[OP1], bytes);

    unsigned int cc = vm->registers[CC]; 
    if (cc >> 30 == 1)
        vm->registers[IP] = vm->registers[CS] | data;

}

void JP(VirtualMachine* vm) {
    int bytes = 4;
    int data = getData(vm, vm->registers[OP1], bytes);

    unsigned int cc = vm->registers[CC];
    if (cc >> 30 == 0) 
        vm->registers[IP] = vm->registers[CS] | data;

}

void JN(VirtualMachine* vm) {
    int bytes = 4;
    int data = getData(vm, vm->registers[OP1], bytes);

    unsigned int cc = vm->registers[CC];
    if (cc >> 30 == 2)
        vm->registers[IP] = vm->registers[CS] | data;

}

void JNZ(VirtualMachine* vm) {
    int bytes = 4;
    int data = getData(vm, vm->registers[OP1], bytes);

    unsigned int cc = vm->registers[CC];
    if (cc >> 30 == 0 || cc >> 30 == 2)
        vm->registers[IP] = vm->registers[CS] | data;

}

void JNP(VirtualMachine* vm) {
    int bytes = 4;
    int data = getData(vm, vm->registers[OP1], bytes);

    unsigned int cc = vm->registers[CC];
    if (cc >> 30 == 1 || cc >> 30 == 2)
        vm->registers[IP] = vm->registers[CS] | data;
}

void JNN(VirtualMachine* vm) {
    int bytes = 4;
    int data = getData(vm, vm->registers[OP1], bytes);

    unsigned int cc = vm->registers[CC];
    if (cc >> 30 == 0 || cc >> 30 == 1)
        vm->registers[IP] = vm->registers[CS] | data;

}

void SYS(VirtualMachine* vm) {
    int bytes = 4;
    int quantity =vm->registers[ECX] & 0xFFFF;
    int size = (vm->registers[ECX] >> 16) & 0xFFFF;
    int mode = vm->registers[EAX];
    int call = getData(vm, vm->registers[OP1], bytes);
    int value;

    if(call == 1) { // read
        int (*reader)() = NULL;

        if (size != 1 && size != 2 && size != 4) {
            error_handler.buildError("Error: tamaño de dato inválido");
            return;
        }

        switch (mode) {
            case 0x01: 
                reader = readDecimal; 
                break;
            case 0x02: 
                reader = readChar; 
                break;
            case 0x04: 
                reader = readOctal; 
                break;
            case 0x08: 
                reader = readHex; 
                break;
            case 0x10: 
                reader = readBinary; 
                break;
            default:
                error_handler.buildError("Error: modo de lectura invalido");
                return;
        }

        for (int i = 0; i < quantity; i++) {
            prepareMemoryAccessHandler(vm, EDX, i*size, size);

            printf("[%04X]: ", vm->registers[MAR] & 0xFFFF);
            value = reader();
            
            prepareMBRHandler(vm, value);
            commitSetMemoryAccess(vm);
        }
    } else if(call == 2) { // write 
        int count = 0;
        writeFunc funcs[5];
        prepareDisplays(mode, funcs, &count);

        for(int i=0; i<quantity; i++) { // preguntar, el original es i = 0; i<quantity
            prepareGetMemoryAccess(vm, EDX, i*size, size);
            value = commitGetMemoryAccess(vm);
            
            printf("[%04X]: ", vm->registers[MAR] & 0xFFFF);
            for (int j = 0; j < count; j++) {
                if (j > 0) printf(" ");
                funcs[j](value);
            }
            printf("\n");
        }

    } else {
        error_handler.buildError("Error: operacion de sistema invalida");
    }
}

void STOP(VirtualMachine* vm) {
    vm->registers[IP] = -1; 
}

void NOT(VirtualMachine* vm) {
    int bytes = 4;
    int data = getData(vm, vm->registers[OP1], bytes);
    setData(vm, vm->registers[OP1], ~data, bytes);
    updateCCRegisterHandler(vm, ~data);
}
