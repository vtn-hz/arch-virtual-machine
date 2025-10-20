#ifndef VM_RUNNER_H
#define VM_RUNNER_H

#include "arguments_parsing.h"

typedef struct VirtualMachine VirtualMachine;

/**
 * Starts execution of the virtual machine.
 * This is the only public interface, all other behavior is internal.
 */
void virtualMachineRun(VirtualMachine*, arguments);

int isSegmentCodeEnded(VirtualMachine*);

void prepareInstruction(VirtualMachine*);

void advanceInstructionPointer(VirtualMachine*);

#endif
