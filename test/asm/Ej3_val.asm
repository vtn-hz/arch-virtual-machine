        LDH [8], 0x8000
        LDL [8], 0x0
        MOV EDX, DS
        LDL ECX, 0x1
        LDH ECX, 0x4
        MOV EAX, 0x1
        SYS 1
OTRO:   CMP [8], 0
        JZ FIN

        MOV [4], [EDX]
        AND [4], [8]
        DIV [4], [8]
        MOV EDX, DS 
        ADD EDX, 4
        LDL ECX, 0x1
        LDH ECX, 0x4
        MOV EAX, 0x1
        SYS 2 
        MOV EDX, DS
        SHR [8], 1
        JMP OTRO
FIN:    STOP