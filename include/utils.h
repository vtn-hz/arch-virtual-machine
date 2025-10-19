#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Aplica una máscara a un número y luego desplaza el resultado.
 *
 * Si `shift_slots` es positivo, el resultado se desplaza a la derecha.
 * Si es negativo, se desplaza a la izquierda (por el valor absoluto de `shift_slots`).
 *
 * @param number Número de entrada.
 * @param mask Máscara a aplicar (bits relevantes).
 * @param shift_slots Cantidad de posiciones a desplazar (positivo = derecha, negativo = izquierda).
 * @return El resultado luego de aplicar la máscara y el desplazamiento.
 */
int applyMask(int number, int mask, int shift_slots);

/**
 * @brief Aplica un desplazamiento a la derecha y luego una máscara.
 *
 * Útil para extraer subconjuntos de bits de una posición determinada.
 *
 * @param number Número de entrada.
 * @param mask Máscara a aplicar después del desplazamiento.
 * @param shiftright_slots Cantidad de bits a desplazar a la derecha antes de aplicar la máscara.
 * @return El valor resultante luego del desplazamiento y la máscara.
 */
int applyMaskAfter(int number, int mask, int shiftright_slots);

/**
 * @brief Extiende el bit de signo de un número.
 *
 * Realiza una extensión de signo, desplazando a la izquierda y luego a la derecha
 * para propagar el bit de signo sobre el tamaño indicado.
 *
 * @param number Número a extender.
 * @param spreadSize Cantidad de bits que se van a extender.
 * @return El número con el signo extendido correctamente.
 */
int spreadSign(int number, int spreadSize);

/**
 * @brief Extrae el sector de operación del operando.
 *
 * Extrae los bits 7 y 8 del operando, que representan el sector de operación.
 *
 * @param operand Operando del cual se extrae el sector.
 * @return Sector de operación (bits 7-8).
 */
int extractOperationSector(int operand);


/**
 * @brief Extrae el codigo del registro.
 *
 * Extrae los bits (0-5) el que representan el indice del registro.
 *
 * @param operand Operando del cual se extrae el indice del registro.
 * @return El indice del registro
 */
int extractRegisterId(int operand);


/**
 * @brief Extrae el tamaño de celda de la operación.
 *
 * Extrae los bits 33 y 34 del operando, que indican el tamaño de la celda.
 *
 * @param operand Operando del cual se extrae el tamaño de celda.
 * @return Tamaño de celda de la operación.
 */
int extractOperationCellSize(int operand);

/**
 * @brief Extrae el tipo de operación del operando.
 *
 * Corresponde a los bits 24–31 del operando.
 *
 * @param operand Operando del cual se extrae el tipo de operación.
 * @return Tipo de operación.
 */
int extractOperationType(int operand);

/**
 * @brief Extrae el valor de la operación del operando.
 *
 * Obtiene los bits inferiores (0–15) y aplica extensión de signo.
 *
 * @param operand Operando del cual se extrae el valor.
 * @return Valor de la operación (con signo extendido).
 */
int extractOperationValue(int operand);

/**
 * @brief Extrae el registro base del operando.
 *
 * Corresponde a los bits 16–23 del operando.
 *
 * @param operand Operando del cual se extrae el registro base.
 * @return Registro base (valor de 8 bits).
 */
int extractOperationBaseRegister(int operand);

/**
 * Converts an integer to its string representation
 * 
 * @param number The integer to convert
 * @return A pointer to the string representation of the integer
 */
char* intToString(int number);

#endif
