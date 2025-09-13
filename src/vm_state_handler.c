#include <stdio.h>

#include "vm_state_handler.h"

#include "virtual_machine.h"

#include "utils.h"

void prepareMemoryAccessHandler (
    VirtualMachine* virtualM, 
    int logicalAddress, int fisicAddress,
    int bytes
) {
    virtualM->registers[LAR] = logicalAddress;
    virtualM->registers[MAR] = (bytes << 16) | fisicAddress;
}

void commitMemoryAccessHandler (
    VirtualMachine* virtualM, int data
) {
    virtualM->registers[MBR] = data;
}

void updateCCRegisterHandler(VirtualMachine* virtualM, int result) {
    virtualM->registers[CC] = 0;

    if (result < 0) 
        virtualM->registers[CC] |= (1 << 31);

    if (result == 0) 
        virtualM->registers[CC] |= (1 << 30);
}