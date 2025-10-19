#ifndef ARGUMENTS_PARSING_H
#define ARGUMENTS_PARSING_H

#define MAX_PARAMS 200

typedef struct arguments{
    char* currentVmx;
    char* currentVmi;
    int memory_size;
    int dissasembler;
    char* params[MAX_PARAMS];
    int paramsAmount;
} arguments;

void getArguments(int argc, char** argv, arguments* args, int sizes[]);

#endif
