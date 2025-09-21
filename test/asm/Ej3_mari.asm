
        LDH EEX, 0x8000
        LDL EEX, 0x0

INICIO: MOV EDX, DS
        LDL ECX, 0x01
        LDH ECX, 0x04
        MOV EAX, 1
        SYS 1

        MOV EBX, [EDX]

OTRO:   CMP EEX, 0
        JZ FIN

        MOV EAX, EBX
        AND EAX, EEX
        DIV EAX, EEX
        MOV [4], EAX

        MOV EDX, DS
        ADD EDX, 4
        LDL ECX, 0x01
        LDH ECX, 0x04
        MOV EAX, 0x01
        SYS 2

        SHR EEX, 1
        JMP OTRO
FIN:    STOP

