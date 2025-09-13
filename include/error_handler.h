#ifndef ERROR_HANDLER_H 
#define ERROR_HANDLER_H

#define NO_ERROR_CONTEXT 0

void fileNotFound       (const char* context, ...);
void invalidHeader      (const char* context, ...);
void invalidInstruction (const char* context, ...);
void segmentationFault  (const char* context, ...);
void divisionByZero     (const char* context, ...);

void customError (const char* context, ...);

typedef void (*p_error_handler)(const char* context, ...);

typedef struct {
    p_error_handler fileNotFound; 
    p_error_handler invalidHeader;
    p_error_handler invalidInstruction;
    p_error_handler segmentationFault;
    p_error_handler divisionByZero;

    p_error_handler customError;
} ErrorHandler;

extern ErrorHandler error_handler;

#endif
