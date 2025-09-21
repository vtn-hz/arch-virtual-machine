; EAX guarda n
; EEX guarda n - 1
; EBX guarda el contador
; ECX guarda el resultado
; Necesito re-iniciar ECX cada vez porque en cada nueva multiplicación, el resultado anterior se sumaba 
; una vez extra como "valor inicial", en lugar de empezar la suma desde 0.
; La multiplicación por suma siempre debe empezar desde 0, no desde el resultado de la multiplicación anterior.

INICIO: MOV EDX,DS
        MOV EAX,1
        LDL ECX,1
        LDH ECX,4
        SYS 1

        MOV EAX, [EDX] ; Obtengo n

        CMP EAX, 1
        JZ FIN
        CMP EAX, 0
        JZ FIN

        MOV EEX, EAX
        SUB EEX, 1 ; Obtengo n - 1
        MOV EBX, EEX ; Inicializo contador
        XOR ECX, ECX ; Inicializo en cero

MULT_LOOP: CMP EBX, 0
	  JZ LOOP
	  ADD ECX, EAX
	  SUB EBX, 1
	  JMP MULT_LOOP

LOOP:   MOV EAX, ECX ; Nuevo n
        SUB EEX, 1   ; Nuevo n - 1
        CMP EEX, 1
        JZ FIN
        MOV EBX, EEX ; Actualizo contador
        XOR ECX, ECX ; Inicializo en cero  
        JMP MULT_LOOP

FIN:    MOV [DS+4], ECX ; esto es para leer de prueba, no va en el ejercicio original
        MOV EDX, DS
        ADD EDX, 4
        LDL ECX, 0x01
        LDH ECX, 0x04
        MOV EAX, 1
        SYS 2  
        STOP
