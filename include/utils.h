#ifndef UTILS_H
#define UTILS_H

/**
 * 
 * @param number 
 * @param mask
 * @param shiftright_slots
 * @return number with mask applied and shifted right by shiftright_slots
 */
int applyMask(int number, int mask, int shiftright_slots);

/**
 * 
 * @param number 
 * @param spreadSize
 * @return number with sign spread to the left by spreadSize
 */
int spreadSign(int number, int spreadSize);

/**
 * Extracts the operation type from the operand.
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
 * Extracts the base register from the operand.
 * 
 * @param operand The operand to extract the base register from.
 * @return The base register (bits 16-23 of the operand).
 */
int extractOperationBaseRegister(int operand);

#endif
