#include <stdlib.h>
#include "vm_runner.h"
#include "error_handler.h"
#include "virtual_machine.h"
#include "segment_table.h"


int isSegmentCodeEnded(VirtualMachine*);

void prepareInstruction(VirtualMachine*);
void advanceInstructionPointer(VirtualMachine*);
void executeInstruction(VirtualMachine*);

void virtualMachineRun(VirtualMachine* virtualM) {
    while (!isSegmentCodeEnded(virtualM)) {
        prepareInstruction(virtualM);
        advanceInstructionPointer(virtualM);
        executeInstruction(virtualM);
    }
}

/**
 * habria que agregarle una capa mas
 * a segment_table, de manera que se pueda
 * utilizar mas sencillo 
 * esta manera podria ser un ejemplo
 */
int isSegmentCodeEnded(VirtualMachine* virtualM) {
    return isLogicalAddressValid(
        virtualM->segment_table, 
        virtualM->registers[ IP ]
    );
}

void prepareInstruction(VirtualMachine* virtualM) {
    /**
     * aca 99% se van a utilizar las funcionalidades
     * de simon
     */
}

void advanceInstructionPointer(VirtualMachine* virtualM) {
    /**
     *  segun el tamaño de la instrucción se avanza la
     *  correspondiente cantidad de offsets al registro IP 
     */
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
