#include "instructions.h"
#include "data_access.h"
#include "virtual_machine.h"
#include "error_handler.h"
#include "utils.h"

void MOV(VirtualMachine* virtualM) {
    int bytes = 4;
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
        error_handler.divisionByZero();
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
