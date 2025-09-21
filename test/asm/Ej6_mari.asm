; EAX contiene resultado de la division
; EEX guarda el signo para el resultado
; EBX guarda el primer operando
; ECX guarda el segundo operando
; AC guarda el resto

INICIO:         MOV EDX, DS
                LDL ECX, 1
                LDH ECX, 4
                MOV EAX, 0x01
                SYS 1

                MOV EBX, [EDX]

                MOV EDX, DS
                ADD EDX, 4
                LDL ECX, 1
                LDH ECX, 4
                MOV EAX, 0x01
                SYS 1

                MOV ECX, [EDX]

                MOV EFX, EBX
                XOR EFX, ECX
                MOV EAX, 0

                CMP EBX, 0
                JNN PRI_POSITIVO
                NOT EBX
                ADD EBX, 1

PRI_POSITIVO:   CMP ECX, 0
                JNN DIV_LOOP
                NOT ECX
                ADD ECX, 1

DIV_LOOP:       CMP EBX, ECX
                JN  FIN_SIG
                ADD EAX, 1
                SUB EBX, ECX
                JMP DIV_LOOP

FIN_SIG:        MOV AC, EBX
                CMP EFX, 0
                JNN FIN
                NOT EAX
                ADD EAX, 1

FIN:    MOV [DS+4], EAX ; esto es para leer de prueba, no va en el ejercicio original
        MOV EDX, DS
        ADD EDX, 4
        LDL ECX, 0x01
        LDH ECX, 0x04
        MOV EAX, 1
        SYS 2
        STOP
