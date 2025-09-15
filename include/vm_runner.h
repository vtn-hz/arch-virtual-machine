#ifndef VM_RUNNER_H
#define VM_RUNNER_H

typedef struct VirtualMachine VirtualMachine;

/**
 * comienza a ejecutar la maquina virtual, es solo visible
 * esta interfaz porque el resto es comportamiento interno
 */
void virtualMachineRun( VirtualMachine* );

int isSegmentCodeEnded(VirtualMachine*);

void prepareInstruction( VirtualMachine* );

void advanceInstructionPointer( VirtualMachine* );

#endif
