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
    return isLogicalAddressValid(virtualM->segment_table, virtualM->registers[IP]) == 0;
}

void prepareInstruction(VirtualMachine* virtualM) {
    int index = transformLogicalAddress(virtualM->segment_table, virtualM->registers[IP]);
    unsigned char byte = virtualM->memory[index];
    
    virtualM->registers[OPC] = byte & 0x1F; // 00011111
    
    byte >>= 4;
    if (byte & 0x01) { // >1 operand
        virtualM->registers[OP1] = (byte & 0x03) << 24;
        virtualM->registers[OP2] = (byte >> 2 & 0x03) << 24;
    } else {
        virtualM->registers[OP1] = (byte >> 2 & 0x03) << 24;
        virtualM->registers[OP2] = 0; // if initialized, this is unnecessary
    }
}

void advanceInstructionPointer(VirtualMachine* virtualM) {    
    int index = transformLogicalAddress(virtualM->segment_table, virtualM->registers[IP]);
    index++; // skips already read instruction
    
    int oparr[2] = {OP1, OP2};
    
    char tp1, tp2;
    tp1 = virtualM->registers[OP1] >> 24 & 0xFF; // 0xFF or 0x03?
    tp2 = virtualM->registers[OP2] >> 24 & 0xFF;
    char optarr[2] = {tp1, tp2};
    
    virtualM->registers[IP] += 1 + tp1 + tp2; // moves ip

    int n = (tp1 > 0) + (tp2 > 0); // number of arguments
    for (int i = n; i > 0; i--) {
        unsigned char byte;
        int opaux = 0; // ends up as 00 ?? ?? ?? (ej: 00 0D 00 04) for the final |=
        
        for (int j = 0; j < optarr[i-1]; j++) {
            opaux <<= 8;
            byte = virtualM->memory[index++];
            opaux |= byte;
        }
        
        if (optarr[i-1] == 2) { // if immediate, preserve sign
            opaux = (opaux << 16) >> 16; // opaux = spreadSign(opaux, 16);
            opaux &= 0x00FFFFFF;
        } 
        
        virtualM->registers[oparr[i-1]] |= opaux; // fills operands
    }
}

void executeInstruction(VirtualMachine* virtualM){
    int requestedInstruction = virtualM->registers[OPC];
    
    if (!(0 <= requestedInstruction && requestedInstruction <= 31)) {
        error_handler.invalidInstruction();
    }

    if (virtualM->instructions[ requestedInstruction ] == NULL) {
        error_handler.invalidInstruction();
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
