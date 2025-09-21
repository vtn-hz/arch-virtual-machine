        MOV EDX, DS
        LDH ECX, 4
        LDL ECX, 1
        MOV EAX, 1

        SYS 1

        MOV [4], [0]
        DIV [4], 2

OTRO:   CMP [4], 1
        JNP FIN
        CMP AC, 0
        JZ FIN

        MOV [8], [0]
        SUB [4], 1
        DIV [8], [4]
        JMP OTRO

FIN:    MOV [12], 0
        CMP [4], 1
        JNZ NOP
        MOV [12], 1

NOP:    MOV EDX, DS
        ADD EDX, 12
        LDH ECX, 4
        LDL ECX, 1
        MOV EAX, 1

        SYS 2

        STOP