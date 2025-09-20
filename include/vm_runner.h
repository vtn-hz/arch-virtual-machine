#ifndef VM_RUNNER_H
#define VM_RUNNER_H

typedef struct VirtualMachine VirtualMachine;

/**
 * Starts execution of the virtual machine.
 * This is the only public interface, all other behavior is internal.
 */
void virtualMachineRun(VirtualMachine*);

int isSegmentCodeEnded(VirtualMachine*);

void prepareInstruction(VirtualMachine*);

void advanceInstructionPointer(VirtualMachine*);

#endif
