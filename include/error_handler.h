#ifndef ERROR_HANDLER_H 
#define ERROR_HANDLER_H

void fileNotFound       ();
void invalidHeader      ();
void invalidInstruction ();
void segmentationFault  ();
void divisionByZero     ();

typedef void(*p_default_error_handler)();
typedef void (*p_custom_error_handler)(const char* context, ...);
typedef void (*p_contexted_error_handler)(p_default_error_handler handler, const char* context, ...);

void customError (const char* context, ...);
void contextedError (p_default_error_handler handler, const char* context, ...);


typedef struct {
    p_default_error_handler fileNotFound; 
    p_default_error_handler invalidHeader;
    p_default_error_handler invalidInstruction;
    p_default_error_handler segmentationFault;
    p_default_error_handler divisionByZero;

    p_custom_error_handler  customError;
    p_contexted_error_handler contextedError;
} ErrorHandler;

extern ErrorHandler error_handler;

#endif
