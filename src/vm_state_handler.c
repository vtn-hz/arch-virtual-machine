#include <stdio.h>

#include "vm_state_handler.h"

#include "virtual_machine.h"

#include "common_registers.h"

#include "segment_table.h"

#include "error_handler.h"

#include "utils.h"

void prepareMemoryAccessHandler(VirtualMachine* virtualM, int baseRegister, int memoryOffset, int bytes) {
    DST segment_table = virtualM->segment_table;
    int logicMemoryAccess = virtualM->registers[baseRegister] + memoryOffset;
    int fisicMemoryAccess = transformLogicalAddress(segment_table, logicMemoryAccess);

    if (!isLogicalAddressValid(segment_table, logicMemoryAccess + (bytes - 1))) 
        error_handler.segmentationFault(logicMemoryAccess);

    virtualM->registers[LAR] = logicMemoryAccess;
    virtualM->registers[MAR] = (bytes << 16) | fisicMemoryAccess;
}

void updateCCRegisterHandler(VirtualMachine* virtualM, int result) {
    virtualM->registers[CC] = 0;

    if (result < 0) 
        virtualM->registers[CC] |= (1 << 31);

    if (result == 0) 
        virtualM->registers[CC] |= (1 << 30);
}