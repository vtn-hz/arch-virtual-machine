#ifndef DATA_ACCESS_H
#define DATA_ACCESS_H

typedef struct VirtualMachine VirtualMachine;

int getData(VirtualMachine*, int operand, int bytes);

void initializeGetters();

#endif