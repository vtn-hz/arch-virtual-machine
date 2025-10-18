#ifndef UTILS_H
#define UTILS_H

/**
 * Applies a mask to a number and shifts the result to the right.
 * 
 * @param number 
 * @param mask
 * @param shiftright_slots
 * @return The masked and right-shifted number.
 */
int applyMask(int number, int mask, int shiftright_slots);

/**
 * Spreads the sign of a number to the left by the specified size.
 * 
 * @param number 
 * @param spreadSize
 * @return The number with the sign spread.
 */
int spreadSign(int number, int spreadSize);

/**
 * Extracts the operation type from an operand.
 * 
 * @param operand The operand to extract the operation type from.
 * @return The operation type (upper 8 bits of the operand).
 */
int extractOperationType(int operand);

/**
 * Extracts the operation value from the operand.
 * 
 * @param operand The operand to extract the operation value from.
 * @return The operation value (lower 16 bits of the operand, with sign preserved).
 */
int extractOperationValue(int operand);

/**
 * Extracts the base register from an operand.
 * 
 * @param operand The operand to extract the base register from.
 * @return The base register (bits 16-23 of the operand).
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
