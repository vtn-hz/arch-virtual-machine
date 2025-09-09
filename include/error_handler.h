#ifndef ERROR_HANDLER_H 
#define ERROR_HANDLER_H

void fileNotFound();
void invalidHeader();
void invalidInstruction();
void segmentationFault();
void divisionByZero();

/** 
*   todo: add more parameters to send
*   exception data   
*/
typedef void(*p_error_handler)();

typedef struct {
    p_error_handler fileNotFound; 
    p_error_handler invalidHeader;
    p_error_handler invalidInstruction;
    p_error_handler segmentationFault;
    p_error_handler divisionByZero;
} ErrorHandler;

extern ErrorHandler error_handler;

#endif
