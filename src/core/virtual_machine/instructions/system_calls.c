#include "system_calls.h"

#include "io_system_calls.h"

#include "virtual_machine.h"

#include "vm_state_handler.h"

#include "data_access.h"

#include "vm_mode.h"

#include "error_handler.h"

#include "utils.h"

#include <stdlib.h>
#include <stdio.h>

void dispatchSystemCall(VirtualMachine *vm) {
    int call = getData(vm, vm->registers[OP1]);

    switch(call){
        case 0x1: systemCallRead(vm);       break; 
        case 0x2: systemCallWrite(vm);      break; 
        case 0x3: systemCallStringRead(vm); break; 
        case 0x4: systemCallStringWrite(vm);break; 
        case 0x7: systemCallClrScreen(vm);  break; 
        case 0xF: systemCallBreakpoint(vm); break; 
        
        default: error_handler.buildError("Error: operacion de sistema invalida");
    }
} 

void systemCallRead(VirtualMachine *vm) {
    int quantity = vm->registers[ECX] & 0xFFFF;
    int size = applyMaskAfter(vm->registers[ECX], 0xFFFF, 16);
    int mode = vm->registers[EAX];

    if (size != 1 && size != 2 && size != 4) {
        error_handler.buildError("Error: tamaño de dato inválido");
        return;
    }

    int (*reader)() = NULL;

    switch (mode) {
        case 0x01: reader = readDecimal; break;
        case 0x02: reader = readChar;    break;
        case 0x04: reader = readOctal;   break;
        case 0x08: reader = readHex;     break;
        case 0x10: reader = readBinary;  break;
        
        default:
            error_handler.buildError("Error: modo de lectura invalido");
    }


    for (int i = 0; i < quantity; i++) {
        prepareMemoryAccessHandler(vm, EDX, i*size, size);

        printf(" [%04X]: ", vm->registers[MAR] & 0xFFFF);
        int value = reader();
        
        prepareMBRHandler(vm, value);
        commitSetMemoryAccess(vm);
    }
}

void systemCallWrite(VirtualMachine *vm) {
    int quantity = vm->registers[ECX] & 0xFFFF;
    int size = applyMaskAfter(vm->registers[ECX], 0xFFFF, 16);
    int mode = vm->registers[EAX];

    int count = 0;
    writeFunc funcs[5], charfunc = writeChar;
    prepareDisplays(mode, funcs, &count);


    for(int i=0; i<quantity; i++) {
        prepareGetMemoryAccess(vm, EDX, i*size, size);
        int value = commitGetMemoryAccess(vm);
        
        printf(" [%04X]: ", vm->registers[MAR] & 0xFFFF);
        for (int j = 0; j < count; j++) {
            if (j > 0) printf(" ");

            if (funcs[j] == charfunc)
                for (int k = size-1; k >= 0; k--)
                    funcs[j](value >> k * 8 & 0xFF);
            else
                funcs[j](value);
        }
        printf("\n");
    }
}

void parseInputString(unsigned char* buffer, int limit) {
    int i = 0;
    int j = 0;

    while( buffer[i] != '\n' && j < limit ) {
        if (buffer[i] == '\\') {
            switch( buffer[i+1] ) {
                case 'n':
                    buffer[j] = '\n';
                    i++;
                break;
                case 't':
                    buffer[j] = '\t';
                    i++;
                break;  
                default:
                    buffer[j] = buffer[i];
            }            
        }else
            buffer[j] = buffer[i];

        j++; i++;
    }

    buffer[j] = '\0';
}

void systemCallStringRead(VirtualMachine* vm) {
    int saveLocation = transformLogicalAddress(vm->segment_table, vm->registers[EDX]);
    int limit = vm->registers[ECX];
    
    if (limit < -1)
        error_handler.buildError("Error: limite invalido para lectura de string");
    
    int hasLimit = limit != -1;

    printf(" [%04X]: ", saveLocation);
    fflush(stdout);

    unsigned char buffer[BUFFER_SIZE];
    fgets((char*) buffer, BUFFER_SIZE, stdin);
    
    parseInputString(buffer, hasLimit ? limit : sizeof(buffer));

    int offset = 0;
    while( buffer[offset] != '\0' ) {
        prepareSetMemoryAccess(vm, EDX, offset, buffer[offset], 1);
        commitSetMemoryAccess(vm);
        offset++;
    }

    prepareSetMemoryAccess(vm, EDX, offset, '\0', 1);
    commitSetMemoryAccess(vm);
}

void systemCallStringWrite(VirtualMachine* vm) {
    unsigned char buffer[BUFFER_SIZE];
    int offset = 0;
    
    do {
        prepareGetMemoryAccess(vm, EDX, offset, 1);
        buffer[offset] = commitGetMemoryAccess(vm);
    } while(buffer[offset++] != '\0');

    printf(" %s", buffer);
}

void systemCallClrScreen(VirtualMachine* vm) {
    system( CLEAR_COMMAND );
}

void systemCallBreakpoint(VirtualMachine* vm) {
    vm->mode = DEBUG_MODE;
}
