; Leo en la posicion 0 de memoria
; Guardo el resultado en la posicion 4 de memoria
; Uso EAX para guardar el numero
; Uso EBX para contar los bits en 1
; Uso EFX para obtener el ultimo bit

INICIO:         MOV EDX, DS
                LDL ECX, 1
                LDH ECX, 4
                MOV EAX, 0x01
                SYS 1

                CMP [EDX], 0
                JN FIN

                XOR AC, AC

                MOV EAX, [EDX]

LOOP:           CMP EAX, 0
                JZ MOSTRAR
                MOV EFX, EAX
                AND EFX, 0x01
                ADD AC, EFX
                SHR EAX, 1
                ;CMP ECX, 0
                ;JZ LOOP
                ;ADD EBX, 1
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