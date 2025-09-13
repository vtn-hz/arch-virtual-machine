#ifndef VM_STATE_HANDLER_H
#define VM_STATE_HANDLER_H

typedef struct VirtualMachine VirtualMachine;

void prepareMemoryAccessHandler (
    VirtualMachine* virtualM, 
    int logicalAddress, int fisicAddress,
    int bytes
);

void commitMemoryAccessHandler (
    VirtualMachine* virtualM, int data
);

void updateCCRegisterHandler(VirtualMachine* virtualM, int result);

#endif
