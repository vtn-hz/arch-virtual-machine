            MOV EDX, DS
            LDL ECX, 1
            LDH ECX, 4
            MOV EAX, 0x01
            SYS 1

            MOV EDX, DS
            ADD EDX, 4
            LDL ECX, 1
            LDH ECX, 4
            MOV EAX, 0x01
            SYS 1

            MOV EBX, 0 ; i
            MOV EFX, 0 ; j

CICLO:      CMP EBX, [0]
            JZ SIGUE     
            CMP EFX, [4]
            JZ FIN_FILA

            
            MOV EAX, EBX ; Formo x (x = i * m + j)
            MUL EAX, [4]
            ADD EAX, EFX

            
            MUL EAX, 4 ; Formo la direccion
            ADD EAX, DS

            
            MOV EDX, EAX ; Leo el valor
            LDL ECX, 1
            LDH ECX, 4
            MOV EAX, 0x01
            SYS 1

            ADD EFX, 1 ; Incremento j
            JMP CICLO

FIN_FILA:   ADD EBX, 1 ; Incremento i
            MOV EFX, 0 ; Reinicio j
            JMP CICLO


 
SIGUE:      MOV EBX, [0] ; El n del vector estara en [n * m * 4 + 4]
            MUL EBX, [4]
            MUL EBX, 4 
            ADD EBX, 4
            MOV [EBX], [4] ; Guardo m (N del vector) en la posicion [n * m * 4 + 4]
            MOV EEX, EBX
            ADD EEX, 4 ; EEX apunta a V[0]


            MOV EBX, 0 ; i
            MOV EFX, 0 ; j

            MOV EAX, 99999
CICLO2:     CMP EFX, [4] ; If(j < m)
            JZ FIN       ; j == m => FIN
            CMP EBX, [0] ; If(i < n)
            JZ FIN_COLUMNA  ; i == n => j++

            ; Formo x (x = i * m + j)
            MOV EDX, EBX
            MUL EDX, [4]
            ADD EDX, EFX

            ; Formo la direccion
            MUL EDX, 4
            ADD EDX, DS
            
            ; Comparo con el minimo
            CMP [EDX], EAX
            JN NUEVO_MIN

            ADD EBX, 1 ; Incremento i
            JMP CICLO2

NUEVO_MIN:  MOV EAX, [EDX]
            ADD EBX, 1
            JMP CICLO2

FIN_COLUMNA: MOV EDX, EFX ; Aca paso el minimo al vector y lo reinicio
            MUL EDX, 4
            ADD EDX, EEX ; EEX + EFX * 4
            MOV [EDX], EAX ; Guardo minimo en la posicion j del vector
            MOV EAX, 1000 
            ADD EFX, 1 ; Incremento j
            MOV EBX, 0 ; Reinicio i
            JMP CICLO2

FIN:        STOP
