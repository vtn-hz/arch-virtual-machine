            MOV AC, 0
            MOV EFX, 0

OTRO:       MOV EDX, DS
            ADD EDX, EFX
            LDL ECX, 1
            LDH ECX, 4
            MOV EAX, 1
            SYS 1
            CMP [EDX], 0
            JN continue
            ADD AC, 1
            ADD EFX, 4
            MOV EEX, [EDX]

            JMP OTRO
CONTINUE:   MOV EBX, AC
            SUB EBX, 1
OTRO_2:     CMP EBX, 0
            JNP FIN
            MOV EDX, DS
            ADD EDX, EFX
            LDL ECX, 1
            LDH ECX, 4
            MOV EAX, 1
            SYS 1

            SUB EBX, 1
            XOR ECX, ECX


OTRO_3:     CMP AC, ECX
            JZ OTRO_2

        
            MOV EAX, ECX
            MUL EAX, 4
            ADD EAX, DS
        
            CMP [EAX], [EDX]
            JNZ NO_EQ
            MOV [EAX], -1
            JMP INC
NO_EQ:      CMP [EAX], 0
            JN INC
            MOV EEX, [EAX]

INC:        ADD ECX, 1
            JMP OTRO_3

FIN:        MOV EDX, DS
            MOV [0], EEX
            LDL ECX, 1
            LDH ECX, 4
            MOV EAX, 1

            SYS 2

            STOP