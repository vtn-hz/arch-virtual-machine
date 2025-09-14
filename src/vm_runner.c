#include <stdlib.h>
#include "vm_runner.h"
#include "error_handler.h"
#include "virtual_machine.h"
#include "common_registers.h"
#include "segment_table.h"
// #include <stdio.h>

int isSegmentCodeEnded(VirtualMachine*);

void prepareInstruction(VirtualMachine*);
void advanceInstructionPointer(VirtualMachine*);
void executeInstruction(VirtualMachine*);

void virtualMachineRun(VirtualMachine* virtualM) {
    while (!isSegmentCodeEnded(virtualM)) {
        prepareInstruction(virtualM);
        advanceInstructionPointer(virtualM);
        // printf("mnm:%08X op1:%08X op2:%08X ip:%08X\n", virtualM->registers[OPC], virtualM->registers[OP1], virtualM->registers[OP2], virtualM->registers[IP]);
        // executeInstruction(virtualM);
    }
}

/**
 * habria que agregarle una capa mas
 * a segment_table, de manera que se pueda
 * utilizar mas sencillo 
 * esta manera podria ser un ejemplo
 */
int isSegmentCodeEnded(VirtualMachine* virtualM) {
    return !isLogicalAddressValid(
        virtualM->segment_table, 
        virtualM->registers[ IP ]
    );
}

void prepareInstruction(VirtualMachine* virtualM) {
    int index = transformLogicalAddress(virtualM->segment_table, virtualM->registers[IP]);
    unsigned char byte = virtualM->memory[index++];
    
    virtualM->registers[OPC] = byte & 0x1F; // 0001 1111
    
    char optarr[2] = {0};
    int n, oparr[2] = {OP1, OP2};

    byte >>= 4;
    if (byte & 0x01) {
        optarr[0] = byte & 0x03;
        optarr[1] = byte >> 2 & 0x03;
        n = 2;
    } else {
        optarr[0] = byte >> 2 & 0x03;
        // optarr[1] = 0;
        n = optarr[0];
    }

    virtualM->registers[OP1] = virtualM->registers[OP2] = 0;
    for (int i = n; i > 0; i--) {
        int opaux = 0;
        
        for (int j = 0; j < optarr[i-1]; j++) {
            opaux <<= 8;
            byte = virtualM->memory[index++];
            opaux |= byte;
        }
        
        if (optarr[i-1] == 2) { // if immediate, preserve sign
            opaux = (opaux << 16) >> 16; // opaux = spreadSign(opaux, 16);
            opaux &= 0x00FFFFFF;
        }

        
        virtualM->registers[oparr[i-1]] = (optarr[i-1] << 24) | opaux; // fills operands (type + value)
    }
}

void advanceInstructionPointer(VirtualMachine* virtualM) {    
    char tp1 = virtualM->registers[OP1] >> 24 & 0xFF;
    char tp2 = virtualM->registers[OP2] >> 24 & 0xFF;

    virtualM->registers[IP] += 1 + tp1 + tp2;
}

void executeInstruction(VirtualMachine* virtualM){
    int requestedInstruction = virtualM->registers[OPC];
    
    if (!(0 <= requestedInstruction && requestedInstruction <= 31)) {
        error_handler.invalidInstruction(requestedInstruction);
    }

    if (virtualM->instructions[ requestedInstruction ] == NULL) {
        error_handler.invalidInstruction(requestedInstruction);
    }

    /**
     * a partir de este momento, la instruccion que haya sido invocada
     * tiene la responsabilidad de: 
     *  - si sus operandos no son los esperados lanzar un error
     *  - actualizar el CC si es necesario
     *  - actualizar el MAR MBR LAR si se accede a memoria
     */
    //virtualM->instructions[ requestedInstruction ]();
}
