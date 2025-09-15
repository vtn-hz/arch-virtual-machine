#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

typedef struct VirtualMachine VirtualMachine;

typedef void(*p_instruction)(VirtualMachine*);

void MOV (VirtualMachine*);
void ADD (VirtualMachine*);
void SUB (VirtualMachine*);
void MUL (VirtualMachine*);
void DIV (VirtualMachine*);
void CMP (VirtualMachine*);
void SHL (VirtualMachine*);
void SHR (VirtualMachine*);
void SAR (VirtualMachine*);  
void AND (VirtualMachine*);
void OR (VirtualMachine*);
void XOR (VirtualMachine*);
void SWAP (VirtualMachine*);
void LDL (VirtualMachine*);
void LDH (VirtualMachine*);
void RND (VirtualMachine*);
void SYS (VirtualMachine*);
void NOT (VirtualMachine*);
void STOP (VirtualMachine*);

#endif
