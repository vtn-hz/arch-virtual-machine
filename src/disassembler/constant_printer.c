#include "constant_printer.h"

#include "virtual_machine.h"

#include "utils.h"

#include "segment_table.h" 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printRawConstantItem(VirtualMachine *vm, int kfisicPointer);

void printParsedConstantItem(VirtualMachine *vm, int kfisicPointer);

int isSegmentConstantEnded(VirtualMachine *vm, int klogicPointer) {
    return !isLogicalAddressValid(vm->segment_table, klogicPointer);
}

void prepareConstantPointer(VirtualMachine *vm, int klogicPointer, int* kfisicPointer) {
    *kfisicPointer = transformLogicalAddress(vm->segment_table, klogicPointer);
}

void printConstantItem(VirtualMachine*  vm, int kfisicPointer) {
    printRawConstantItem(vm, kfisicPointer);
    printf(" | ");
    printParsedConstantItem(vm, kfisicPointer);
    printf("\n");
}

void advanceConstantPointer(VirtualMachine *vm,  int* klogicPointer, int kfisicPointer) {
    *klogicPointer += strlen((char*) (vm->memory + kfisicPointer)) + 1; 
}

void printParsedConstantItem(VirtualMachine *vm, int kfisicPointer) {
    printf("\"");
    while( vm->memory[kfisicPointer] ) {
        char c = (char) vm->memory[kfisicPointer];
        printf( c >= 32 && c < 128 ? "%c" : "." , c);
        kfisicPointer++;
    }
    printf("\"");
}   

void printRawConstantItem(VirtualMachine *vm, int kfisicPointer) {
    int marginLeft = 8;
    int totalSize = 0;

    printf(" [%04X]:", kfisicPointer);

    while( vm->memory[kfisicPointer] && totalSize < 6 ) {
        printf(" %02X", vm->memory[kfisicPointer]);
        kfisicPointer++;
        totalSize++;
    }

    if ( vm->memory[kfisicPointer] )
        printf(" ..");
    else 
        printf(" 00");
    totalSize++;

    int bytesRestantes = marginLeft - totalSize;
    for(int i=0; i < bytesRestantes ; i++) 
        printf("   ");    
}
