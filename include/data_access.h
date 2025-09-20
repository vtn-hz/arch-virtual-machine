#ifndef DATA_ACCESS_H
#define DATA_ACCESS_H

typedef struct VirtualMachine VirtualMachine;

int  getData(VirtualMachine*, int operand, int bytes);
void prepareGetMemoryAccess(VirtualMachine*, int baseRegister, int memoryOffset, int bytes);
int  commitGetMemoryAccess(VirtualMachine*);

void setData(VirtualMachine*, int operand, int value, int bytes);
void prepareSetMemoryAccess(VirtualMachine*, int baseRegister, int memoryOffset, int value, int bytes);
void commitSetMemoryAccess(VirtualMachine*);

typedef int (*p_getter_data)(VirtualMachine*, int operand, int bytes);
typedef void (*p_setter_data)(VirtualMachine*, int operand, int value, int bytes);

void initializeGetters();
void initializeSetters();

#endif