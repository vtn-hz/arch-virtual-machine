        MOV AC, 0
        MOV EFX, 0

OTRO:   MOV EDX, DS
        LDL ECX, 1
        LDH ECX, 4
        MOV EAX, 1

        SYS 1

        CMP [EDX], 0
        JN SIG

        ADD EFX, [EDX]
        ADD AC, 1

        JMP OTRO

SIG:    CMP AC, 1
        JNP FIN

        MOV EDX, DS
        LDL ECX, 1
        LDH ECX, 4
        MOV EAX, 1

        SYS 1

        SUB AC, 1
        SUB EFX, [EDX]
        JMP SIG

FIN:    MOV EDX, DS
        MOV [DS], EFX
        LDL ECX, 1
        LDH ECX, 4
        MOV EAX, 1

        SYS 2

        STOP
        