        MOV [4], 1
        MOV EDX, DS
        LDH ECX, 4
        LDL ECX, 1
        MOV EAX, 1
        SYS 1
        CMP [DS], 0
        JNP FIN
        MOV AC, 1
OTRO:   CMP [DS], AC
        JN FIN
        MOV [8], [4]
        MOV [12], AC
        ADD AC, 1
        MOV [4], 0
        otro_2: CMP [12], 0
        JZ otro

        MOV [16], [12]
        AND [16], 1
        CMP [16], 0

        JZ nosuma
        ADD [4], [8]
NOSUMA: SHL [8], 1
        SHR [12], 1
        JMP otro_2
FIN:    MOV EDX, DS
        ADD EDX, 4
        LDH ECX, 4
        LDL ECX, 1
        MOV EAX, 1
        SYS 2
        STOP
