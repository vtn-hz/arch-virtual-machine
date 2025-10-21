#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_handler.h"

#include "virtual_machine.h"

#include "vm_printer.h"
#include "vm_runner.h"

#include "arguments_parsing.h"
#include "files_parsing.h"

int main(int argc, char** argv) {
    arguments args;
    int sizes[6] = {0, 0, 0, 0, 0, 0}; //order: param, const, code, data, extra, stack (same as to be saved in memory)

    getArguments(argc, argv, &args, sizes); // param segment size
    
    // printf("Archivo .vmx: %s\n", args.currentVmx);
    // printf("Archivo .vmi: %s\n", args.currentVmi);
    // printf("Tamaño memoria: %d KB\n", args.memory_size);
    // printf("Disasembler: %d\n", args.dissasembler);
    // printf("Cantidad de parametros: %d\n", args.paramsAmount);
    // for (int i = 0; i < args.paramsAmount; i++)
    // printf("Parametro %d: %s\n", i + 1, args.params[i]);
    
    VirtualMachine* virtualM = buildVm(&args, sizes);

    if (args.dissasembler) 
        virtualMachinePrint(virtualM);
    virtualMachineRun(virtualM, args);
    
    releaseVm(virtualM);
    return 0;
}

