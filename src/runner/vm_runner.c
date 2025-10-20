#include <stdlib.h>
#include <stdio.h>

#include "vm_runner.h"

#include "error_handler.h"

#include "virtual_machine.h"
#include "utils.h"

#include "common_registers.h"
#include "segment_table.h"

#include "mnemonics_str.h"

#include "vm_mode.h"

int isSegmentCodeEnded(VirtualMachine*);

void prepareInstruction(VirtualMachine*);
void advanceInstructionPointer(VirtualMachine*);
void executeInstruction(VirtualMachine*);

void virtualMachineRun(VirtualMachine* virtualM, arguments args) {
    while (!isSegmentCodeEnded(virtualM)) {

        if ( isDebugMode(virtualM, args.params) ) {
            solveDebugAction(virtualM, args, askDebugAction());
        }
      
        prepareInstruction(virtualM);
        advanceInstructionPointer(virtualM);
        executeInstruction(virtualM);  
        printf("%04X\n", virtualM->registers[IP] & 0xFFFF);
    }
}

int isSegmentCodeEnded(VirtualMachine* virtualM) {
    return !isLogicalAddressValid(
        virtualM->segment_table, 
        virtualM->registers[IP]
    );
}

void prepareInstruction(VirtualMachine* virtualM) {
    int index = transformLogicalAddress(virtualM->segment_table, virtualM->registers[IP]);
    unsigned char byte = virtualM->memory[index++];
    
    virtualM->registers[OPC] = byte & 0x1F;
    
    char optarr[2] = {0};
    int n, oparr[2] = {OP1, OP2};

    byte >>= 4;
    if (byte & 0x01) {
        optarr[0] = byte & 0x03;
        optarr[1] = byte >> 2 & 0x03;
        n = 2;
    } else {
        optarr[0] = byte >> 2 & 0x03;
        n = optarr[0] > 0; // 1 if there is an operand, 0 otherwise
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
            opaux = spreadSign(opaux, 16);
            opaux &= 0x00FFFFFF;
        }

        
        virtualM->registers[oparr[i-1]] = (optarr[i-1] << 24) | opaux;
    }
}

void advanceInstructionPointer(VirtualMachine* virtualM) {    
    char tp1 = virtualM->registers[OP1] >> 24 & 0xFF;
    char tp2 = virtualM->registers[OP2] >> 24 & 0xFF;

    virtualM->registers[IP] += 1 + tp1 + tp2;
}

void executeInstruction(VirtualMachine* virtualM) {
    int requestedInstruction = virtualM->registers[OPC];
    
    if (!(0 <= requestedInstruction && requestedInstruction <= 31))
        error_handler.invalidInstruction(requestedInstruction);

    if (virtualM->instructions[requestedInstruction] == NULL)
        error_handler.invalidInstruction(requestedInstruction);

    virtualM->instructions[requestedInstruction](virtualM);
}
