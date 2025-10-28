#ifndef ERROR_HANDLER_H 
#define ERROR_HANDLER_H

void fileNotFound();
void invalidHeader();

void invalidInstruction(int);
void invalidOperand(int);
void emptyOperand();

void segmentationFault(int);
void divisionByZero(int, int);

void stackOverflow();
void stackUnderflow();

void buildError(const char* context, ...);

typedef void (*p_void_error_handler)(void);
typedef void (*p_int_error_handler)(int);
typedef void (*p_int_int_error_handler)(int, int);

typedef void (*p_custom_error_handler)(const char* context, ...);

typedef struct {
    p_void_error_handler fileNotFound; 
    p_void_error_handler invalidHeader;

    p_int_error_handler invalidInstruction;
    p_int_error_handler invalidOperand;
    p_void_error_handler emptyOperand;

    p_int_error_handler segmentationFault;
    p_int_int_error_handler divisionByZero;

    p_void_error_handler stackOverflow;
    p_void_error_handler stackUnderflow;

    p_custom_error_handler buildError;
} ErrorHandler;

extern ErrorHandler error_handler;

#endif
