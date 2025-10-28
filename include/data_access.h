#ifndef DATA_ACCESS_H
#define DATA_ACCESS_H

typedef struct VirtualMachine VirtualMachine;

#define DEFAULT_ACCESS_SIZE 4

int  getData(VirtualMachine*, int operand);
void prepareGetMemoryAccess(VirtualMachine*, int baseRegister, int memoryOffset, int bytes);
int  commitGetMemoryAccess(VirtualMachine*);

int executeDataPop(VirtualMachine*);

void setData(VirtualMachine*, int operand, int value);
void prepareSetMemoryAccess(VirtualMachine*, int baseRegister, int memoryOffset, int value, int bytes);
void commitSetMemoryAccess(VirtualMachine*);

void executeDataPush(VirtualMachine*, int);

typedef int (*p_getter_data)(VirtualMachine*, int operand);
typedef void (*p_setter_data)(VirtualMachine*, int operand, int value);

void initializeGetters();
void initializeSetters();

#endif