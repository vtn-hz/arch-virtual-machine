#ifndef SYSTEM_CALLS_H
#define SYSTEM_CALLS_H

typedef struct VirtualMachine VirtualMachine;

#define BUFFER_SIZE 256

void dispatchSystemCall( VirtualMachine* );

void systemCallRead( VirtualMachine* );

void systemCallWrite( VirtualMachine* );

void systemCallStringRead( VirtualMachine* );

void systemCallStringWrite( VirtualMachine* );

#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
#else   
    #define CLEAR_COMMAND "clear"
#endif

void systemCallClrScreen( VirtualMachine* );

void systemCallBreakpoint( VirtualMachine* );


#endif