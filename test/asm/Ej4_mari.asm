; EBX contiene el valor de A
; ECX contiene el valor de B
; EAX tendra el resultado de A * B

        MOV EDX, DS     ;leo primer operando
        LDL ECX, 0x01
        LDH ECX, 0x04
        MOV EAX, 1
        SYS 1

        MOV EDX, DS     ;lee segundo operando
        ADD EDX, 4
        LDL ECX, 0x01
        LDH ECX, 0x04
        MOV EAX, 1
        SYS 1

        MOV EBX, [0]
        MOV ECX, [4]

        XOR EAX, EAX

        CMP EBX, 0
        JNN OTRO
        NOT EBX
        ADD EBX, 1
        NOT ECX
        ADD ECX, 1

OTRO:   CMP EBX, 0
        JZ PRINT        ; deberia saltar a fin
        ADD EAX, ECX
        SUB EBX, 1
        JMP OTRO

PRINT:  MOV [DS+4], EAX ; esto es para leer de prueba, no va en el ejercicio original
        MOV EDX, DS
        ADD EDX, 4
        LDL ECX, 0x01
        LDH ECX, 0x04
        MOV EAX, 1
        SYS 2
FIN:    STOP            ; esta es la unica linea de fin en el original
