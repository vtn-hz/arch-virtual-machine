; Leo en la posicion 0 de memoria
; Guardo el resultado en la posicion 4 de memoria
; Uso EAX para guardar el numero
; Uso AC para contar los bits en 1
; Uso EFX para obtener el ultimo bit
; Uso EEX para guardar la mascara

                MOV EDX, DS
                ADD EDX, 4
                LDL ECX, 1
                LDH ECX, 4
                MOV EAX, 0x01
                SYS 1

                MOV EEX, [4]

INICIO:         MOV EDX, DS
                LDL ECX, 1
                LDH ECX, 4
                MOV EAX, 0x01
                SYS 1
                CMP [EDX], 0
                JN FIN

                MOV EAX, [EDX]

                XOR AC, AC
                AND EAX, EEX

LOOP:           CMP EAX, 0
                JZ MOSTRAR
                MOV EFX, EAX
                AND EFX, 0x01
                ADD AC, EFX
                SHR EAX, 1
                JMP LOOP

MOSTRAR:        MOV EDX, DS
                ADD EDX, 4
                MOV [EDX], AC
                LDL ECX, 1
                LDH ECX, 4
                MOV EAX, 0x01
                SYS 2
                JMP INICIO

FIN:            STOP