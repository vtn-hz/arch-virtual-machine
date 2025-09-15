#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "instructions.h"

#include "data_access.h"

#include "virtual_machine.h"

#include "error_handler.h"

#include "utils.h"

#include "system_calls.h"

#include "vm_state_handler.h"

void MOV(VirtualMachine* virtualM) {
    int bytes = 1;
    int data = getData(virtualM, virtualM->registers[OP2], bytes);
    setData(virtualM, virtualM->registers[OP1], data, bytes);
}

void ADD(VirtualMachine* virtualM) {
    int bytes = 4;
    int data1 = getData(virtualM, virtualM->registers[OP1], bytes);
    int data2 = getData(virtualM, virtualM->registers[OP2], bytes);
    setData(virtualM, virtualM->registers[OP1], data1 + data2, bytes);
    updateCCRegisterHandler(virtualM, data1 + data2);
}

void SUB(VirtualMachine* virtualM) {
    int bytes = 4;
    int data1 = getData(virtualM, virtualM->registers[OP1], bytes);
    int data2 = getData(virtualM, virtualM->registers[OP2], bytes);
    setData(virtualM, virtualM->registers[OP1], data1 - data2, bytes);
    updateCCRegisterHandler(virtualM, data1 - data2);
}

void MUL(VirtualMachine* virtualM) {
    int bytes = 4;
    int data1 = getData(virtualM, virtualM->registers[OP1], bytes);
    int data2 = getData(virtualM, virtualM->registers[OP2], bytes);
    setData(virtualM, virtualM->registers[OP1], data1 * data2, bytes);
    updateCCRegisterHandler(virtualM, data1 * data2);
}

void DIV(VirtualMachine* virtualM) {
    int bytes = 4;
    int data1 = getData(virtualM, virtualM->registers[OP1], bytes);
    int data2 = getData(virtualM, virtualM->registers[OP2], bytes);
    if (data2 == 0) {
        error_handler.divisionByZero(data1, data2);
    }
    setData(virtualM, virtualM->registers[OP1], data1 / data2, bytes);
    updateCCRegisterHandler(virtualM, data1 / data2);
    virtualM->registers[AC] = data1 % data2; 
}

void CMP(VirtualMachine* virtualM) {
    int bytes = 4;
    int data1 = getData(virtualM, virtualM->registers[OP1], bytes);
    int data2 = getData(virtualM, virtualM->registers[OP2], bytes);
    updateCCRegisterHandler(virtualM, data1 - data2);
}

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

    int loadedHigh = (dataOp1 & 0x0000FFFF) | ((dataOp2 & 0x0000FFFF) << 16);
    setData(vm, vm->registers[OP1], loadedHigh, bytes);
}

void RND(VirtualMachine* vm) {
    int bytes = 4;

    int dataOp2 = getData(vm, vm->registers[OP2], bytes);

    srand(time(NULL));
    int randomValue = rand() % (dataOp2 + 1);

    setData(vm, vm->registers[OP1], randomValue, bytes);
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
void SYS(VirtualMachine* virtualM){
    int bytes = 4;
    int pos = EDX<<16;
    int quantity =virtualM->registers[ECX] & 0xFFFF;
    int size = (virtualM->registers[ECX] >> 16) & 0xFFFF;
    //int size = (unsigned int)virtualM->registers[ECX] >> 16;
    int mode = virtualM->registers[EAX];
    int call = getData(virtualM, virtualM->registers[OP1], bytes);
    //printf("EDX %08X\n",pos);
    //printf("tipo %d\n",call);
    //printf("size %d\n",size);
    int value;
    if(call==1) //read
    { 
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
            value = reader();
            setDataToMemory(virtualM, pos+i*size, value, size);
        }
    }
    else if(call==2) //write
    { 
        void (*writer)(int) = NULL;

        switch (mode) {
            case 0x01: 
                writer = writeDecimal; 
                break;
            case 0x02: 
                writer = writeChar; 
                break;
            case 0x04: 
                writer = writeOctal; 
                break;
            case 0x08: 
                writer = writeHex; 
                break;
            case 0x10: 
                writer = writeBinary; 
                break;
            default:
                error_handler.buildError("Error: modo de escritura invalido");
                return;
        }
        
        for(int i=-quantity; i<0; i++) { //preguntar, el original es i = 0; i<quantity
            value = getDataFromMemory(virtualM, pos+i*size, size);
            writer(value);
        }
    }
    else {
        error_handler.buildError("Error: operacion de sistema invalida");
    }
}

void STOP(VirtualMachine* virtualM) {
    virtualM->registers[IP] = -1; 
}

void NOT(VirtualMachine* virtualM) {
    int bytes = 4;
    int data = getData(virtualM, virtualM->registers[OP1], bytes);
    setData(virtualM, virtualM->registers[OP1], ~data, bytes);
}
