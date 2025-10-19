#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files_parsing.h"

#include "error_handler.h"

#include "arguments_parsing.h"

#include "virtual_machine.h"

void getParsed(char** codeSegmentContent, char** constSegmentContent, arguments* args, int sizes[], int* entryPoint, int regs[], int segs[]) {    
    FILE* file;
    int version;
    
    if(args->currentVmx)
        file = fopen(args->currentVmx, "rb");
    else
        file = fopen(args->currentVmi, "rb");

    if (file == NULL)
        error_handler.fileNotFound();

    readIdentifier(file, &version);

    if (args->currentVmx)
        if(version == 2)
            vmxVersionTwo(file, codeSegmentContent, constSegmentContent, entryPoint, sizes);
        
        else
            vmxVersionOne(file, codeSegmentContent, sizes);   
    
    else
        //vmiVersionOne(file, codeSegmentContent, args, regs, segs);
    
    fclose(file);
}

void vmxVersionOne(FILE* file, char **codeSegmentContent, int sizes[]){
    char codeSize[3];

    fread(codeSize, sizeof(char), 2, file); //code size
    sizes[2] = (unsigned char)codeSize[0] << 8 | (unsigned char)codeSize[1];

    *codeSegmentContent = (char*) malloc(sizes[2]);
    fread(*codeSegmentContent, sizeof(char), sizes[2], file);

    sizes[3] = DEFAULT_MEMORY_SIZE - sizes[2]; //rest of memory is data segment
}

void vmxVersionTwo(FILE* file, char **codeSegmentContent, char** constSegmentContent, int* entryPoint, int sizes[]){
    char temp[3];

    readSizes(file, sizes);
    fread(temp, sizeof(char), 2, file); 
    *entryPoint = (unsigned char)temp[0] << 8 | (unsigned char)temp[1];

    *codeSegmentContent = (char*) malloc(sizes[2]);
    fread(*codeSegmentContent, sizeof(char), sizes[2], file);
    
    if(sizes[1]){
        *constSegmentContent = (char*) malloc(sizes[1]);
        fread(*constSegmentContent, sizeof(char), sizes[1], file);    
    }
}

void vmiVersionOne(FILE* file, char** fileContent, arguments* args, int regs[], int segs[]){
    char temp[5];

    fread(temp, sizeof(char), 2, file); //memory size
    args->memory_size = (unsigned char)temp[0] << 8 | (unsigned char)temp[1];

    for(int i = 0; i < 32; i++){
        fread(temp, sizeof(char), 4, file); //i do this here 'cause of the big endian issue
        regs[i] =(unsigned char)temp[0] << 24 | (unsigned char)temp[1] << 16 | (unsigned char)temp[2] << 8 | (unsigned char)temp[3]; 
    }

    for(int i = 0; i < 8; i++){
        fread(temp, sizeof(char), 4, file); 
        segs[i] = (unsigned char)temp[0] << 24 | (unsigned char)temp[1] << 16 | (unsigned char)temp[2] << 8 | (unsigned char)temp[3]; 
    }

    *fileContent = (char*) malloc(args->memory_size*1024);
    fread(fileContent, sizeof(char), args->memory_size*1024, file); //we read the whole memory content here, not just the code segment   

}

void readSizes(FILE* file, int sizes[]) {
    char temp[3];
    fread(temp, sizeof(char), 2, file); //code
    sizes[2] = (unsigned char)temp[0] << 8 | (unsigned char)temp[1]; //&sizes[2]

    fread(temp, sizeof(char), 2, file); //data
    sizes[3] = (unsigned char)temp[0] << 8 | (unsigned char)temp[1];

    fread(temp, sizeof(char), 2, file); //extra
    sizes[4] = (unsigned char)temp[0] << 8 | (unsigned char)temp[1];

    fread(temp, sizeof(char), 2, file); //stack
    sizes[5] = (unsigned char)temp[0] << 8 | (unsigned char)temp[1];

    fread(temp, sizeof(char), 2, file); //const
    sizes[1] = (unsigned char)temp[0] << 8 | (unsigned char)temp[1];
}

//*sizeFile = (unsigned char)title[6] << 8 | (unsigned char)title[7];

void readIdentifier(FILE* file, int* version) { // assumes file exists
    char title[6];

    fread(title, sizeof(char), 6, file);

    if(strncmp(title, "VMX25", 5)!=0 && strncmp(title, "VMI25",5)!=0)
        error_handler.invalidHeader();

    *version = (unsigned char)title[5] - '0';
}
