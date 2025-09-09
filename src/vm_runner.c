#include "vm_runner.h"

#include "error_handler.h"
#include "virtual_machine.h"

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
 * 
 * esta manera podria ser un ejemplo
 */
int isSegmentCodeEnded(VirtualMachine* virtualM) {
    /** 
     * lo ideal seria solamente tener que enviar 
     * la memoria y que automaticamente detecte
     * el segmento con la validacion que exista el 
     * segmento.
     */
    int ipContent = virtualM->reg[ CS ]; 

    // se podria hacer un /utils para esto
    // tiene pinta que se va a usar permanentemente
    int stIndex = (address >> 16) & 0xFFFF;
    int offset = address & 0xFFFF;
    
    return isAccessAllowed(virtualM->table_seg[stIndex], offset);
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
    int requestedInstruction = virtualM->reg[OPC];
    
    if (!(0 <= requestedInstruction && requestedInstruction <= 31)) {
        error_handler.invalidInstruction();
    }

    if (virtualM->p_instructions[ requestedInstruction ] == NULL) {
        error_handler.invalidInstruction();
    }

    /**
     * a partir de este momento, la instruccion que haya sido invocada
     * tiene la responsabilidad de: 
     *  - si sus operandos no son los esperados lanzar un error
     *  - actualizar el CC si es necesario
     *  - actualizar el MAR MBR LAR si se accede a memoria
     */
    virtualM->p_instructions[ requestedInstruction ]();
}

