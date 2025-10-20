#ifndef COSNTANT_PRINTER_H
#define COSNTANT_PRINTER_H

typedef struct VirtualMachine VirtualMachine;

int isSegmentConstantEnded(VirtualMachine*, int klogicPointer);

void prepareConstantPointer(VirtualMachine*, int klogicPointer, int* kfisicPointer);

void printConstantItem(VirtualMachine*, int kfisicPointer);

void advanceConstantPointer(VirtualMachine*, int* klogicPointer, int kfisicPointer);

#endif
