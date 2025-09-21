        MOV EDX, DS
        LDL ECX, 1
        LDH ECX, 4
        MOV EAX, 1
        SYS 1       ; read N m[N][]

        MOV EDX, DS
        ADD EDX, 4
        LDL ECX, 1
        LDH ECX, 4
        MOV EAX, 1
        SYS 1       ; read M m[N][M]

        XOR EEX, EEX    ; i=0

LOOP_I: CMP [0], EEX    
        JNP GET_VEC
        MOV EDX, EEX
        MUL EDX, [4]
        XOR EFX, EFX    ; j=0

LOOP_J: CMP [4], EFX
        JNP FIN_LOOPJ
        MOV EDX, EEX
        MUL EDX, [4]
        ADD EDX, EFX
        MUL EDX, 4

        ADD EDX, DS
        ADD EDX, 8

        LDL ECX, 1
        LDH ECX, 4
        MOV EAX, 1
        SYS 1
        ADD EFX, 1      ; incremento j+1
        JMP LOOP_J

FIN_LOOPJ: ADD EEX, 1   ; incremento i en 1
        JMP LOOP_I

GET_VEC: MOV EDX, [4]   ; cant columnas
        MUL EDX, [0]    ; cant filas
        ADD EDX, 2      ; añado el espacio utilizado por N  M
        MUL EDX, 4
        ADD EDX, DS

        MOV [EDX],[4]   ; guardo tam array
        XOR EAX, EAX     ; eax = 0 

        MOV EEX, [4]    
        MUL EEX, 4      ; EEX = M * 4 (incremento entre filas)

        
CALC_VEC: CMP EAX, [4]
        JNN PRINT_VEC
        ADD EDX, 4
        MOV [EDX], 0xFF
        MOV ECX, DS
        ADD ECX, 8      ; apunto al principio de la matriz
        ;ADD ECX, EAX    ; apunto al valor a calcular (?)
        MOV EBX, EAX    ; EBX = índice de columna
        MUL EBX, 4      ; EBX = índice * 4
        ADD ECX, EBX 

        XOR EBX, EBX

FIND_MIN: CMP EBX, [0]
        JNN CALC_VEC
        ADD EBX, 1

        CMP [EDX], [ECX]
        JN INC
        ADD EAX, 1      ; si lo hago en el ciclo de arriba, no me cuenta la columna 0
        MOV [EDX], [ECX]

INC:    ADD ECX, EEX
        JMP FIND_MIN

        ; Imprimir el vector de mínimos
PRINT_VEC: MOV EDX, [4]     ; EDX = M (cantidad de columnas)
           MUL EDX, [0]     ; EDX = M * N 
           ADD EDX, 2       ; EDX = M*N + 2
           MUL EDX, 4       ; EDX = (M*N + 2) * 4
           ADD EDX, DS      ; EDX apunta al inicio del vector
           ADD EDX, 4       ; EDX apunta al primer elemento (saltea el tamaño)
           
           XOR EAX, EAX     ; EAX = 0 (contador de columnas)

PRINT_LOOP: CMP EAX, [4]    ; EAX < M?
            JZ FIN          ; Si terminé todas las columnas, fin
            
            ; Calcular dirección del elemento vector[EAX]
            MOV EFX, [4]    ; EFX = M
            MUL EFX, [0]    ; EFX = M * N 
            ADD EFX, 2      ; EFX = M*N + 2
            MUL EFX, 4      ; EFX = (M*N + 2) * 4
            ADD EFX, DS     ; EFX apunta al inicio del vector
            ADD EFX, 4      ; EFX apunta al primer elemento
            
            MOV EBX, EAX    ; EBX = contador actual
            MUL EBX, 4      ; EBX = contador * 4
            ADD EFX, EBX    ; EFX apunta a vector[EAX]
            
            ; Copiar el valor a una posición temporal para impresión
            MOV EBX, [EFX]  ; EBX = vector[EAX]
            
            ; Buscar posición temporal libre (al final de todo)
            MOV EDX, [4]    ; EDX = M
            MUL EDX, [0]    ; EDX = M * N (elementos de matriz)
            ADD EDX, 2      ; EDX = M*N + 2 (suma N y M)
            ADD EDX, [4]    ; EDX = M*N + 2 + M (suma tamaño vector)
            ADD EDX, 1      ; EDX = M*N + 2 + M + 1 (posición libre)
            MUL EDX, 4      ; EDX = bytes
            ADD EDX, DS     ; EDX apunta a posición temporal
            
            MOV [EDX], EBX  ; Guardar valor en posición temporal
            
            ; Configurar para SYS 2 (write)
            LDL ECX, 1
            LDH ECX, 4
            MOV EEX, EAX    ; Guardar contador en EEX
            MOV EAX, 1      ; SYS 2 para write
            SYS 2
            MOV EAX, EEX    ; Restaurar contador desde EEX
            
            ; Siguiente elemento
            ADD EAX, 1      ; Incrementar contador
            JMP PRINT_LOOP

FIN:    STOP
