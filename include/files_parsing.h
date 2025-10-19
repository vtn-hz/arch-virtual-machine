#include "arguments_parsing.h"

#ifndef FILES_PARSING_H
#define FILES_PARSING_H

void getParsed(char** codeSegmentContent, char** constSegmentContent,  arguments* args, int sizes[], int* entryPoint, int regs[], int segs[]);
void vmxVersionOne(FILE* file, char **codeSegmentContent, int sizes[]);
void vmxVersionTwo(FILE* file,char **codeSegmentContent, char** constSegmentContent, int* entryPoint, int sizes[]);
void vmiVersionOne(FILE* file, char** fileContent, arguments* args, int regs[], int segs[]);
void readSizes(FILE* file, int sizes[]);
void readIdentifier(FILE* file, int* version);

#endif


