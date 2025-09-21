#ifndef COMMON_REGISTERS_H
#define COMMON_REGISTERS_H

/**
 * Common registers used by the virtual machine.
 * See: https://learn.microsoft.com/es-es/cpp/c-language/c-enumeration-declarations?view=msvc-170
 */
enum CommonRegisters {
    LAR = 0,
    MAR = 1,
    MBR = 2,
    IP  = 3,
    OPC = 4,
    OP1 = 5,
    OP2 = 6,
    EAX = 10,
    EBX = 11,
    ECX = 12,
    EDX = 13,
    EEX = 14,
    EFX = 15,
    AC  = 16,
    CC  = 17,
    CS  = 26,
    DS  = 27
};

#endif
