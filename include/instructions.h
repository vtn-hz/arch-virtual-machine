#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

typedef struct VirtualMachine VirtualMachine;

typedef void(*p_instruction)(VirtualMachine*);

#endif

void instructor();