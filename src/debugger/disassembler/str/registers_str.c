#include "registers_str.h"
#include <stddef.h>

const char* REGISTERS_STR[32] = {
    "LAR", // 0x00
    "MAR", // 0x01
    "MBR", // 0x02
    "IP",  // 0x03
    "OPC", // 0x04
    "OP1", // 0x05
    "OP2", // 0x06
    "SP",  // 0x07
    "BP",  // 0x08
    NULL,  // 0x09
    "EAX", // 0x0A
    "EBX", // 0x0B
    "ECX", // 0x0C
    "EDX", // 0x0D
    "EEX", // 0x0E
    "EFX", // 0x0F
    "AC",  // 0x10
    "CC",  // 0x11
    NULL,  // 0x12
    NULL,  // 0x13
    NULL,  // 0x14
    NULL,  // 0x15
    NULL,  // 0x16
    NULL,  // 0x17
    NULL,  // 0x18
    NULL,  // 0x19
    "CS",  // 0x1A
    "DS",  // 0x1B
    "ES",  // 0x1C
    "SS",  // 0x1D
    "KS",  // 0x1E
    "PS",  // 0x1F
};

const char* REGISTERS_STR_SECTOR[6][4] = {
  {"EAX", "AL", "AH", "AX"},
  {"EBX", "BL", "BH", "BX"},
  {"ECX", "CL", "CH", "CX"},
  {"EDX", "DL", "DH", "DX"},
  {"EEX", "EL", "EH", "EX"},
  {"EFX", "FL", "FH", "FX"}
};
