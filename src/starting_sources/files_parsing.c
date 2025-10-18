#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files_parsing.h"

#include "error_handler.h"

#include "arguments_parsing.h"

#include "virtual_machine.h"

void getParsed(char** codeSegmentContent, char** constSegmentContent, arguments* args, int sizes[], int* entryPoint) {    
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
        //vmiVersionOne(file, codeSegmentContent, args, sizes);
    
    fclose(file);
}

void vmxVersionOne(FILE* file, char **codeSegmentContent, int sizes[]){
    int codeSize;

    fread(&codeSize, sizeof(short int), 1, file); //code size
    sizes[2] = codeSize; //could be replace with fread(&sizes[2], sizeof(short int), 1, file);

    *codeSegmentContent = (char*) malloc(sizes[2]);
    fread(*codeSegmentContent, sizeof(char), sizes[2], file);

    sizes[3] = DEFAULT_MEMORY_SIZE - sizes[2]; //rest of memory is data segment
}

void vmxVersionTwo(FILE* file,char **codeSegmentContent, char** constSegmentContent, int* entryPoint, int sizes[]){
    
    readSizes(file, sizes);
    fread(entryPoint, sizeof(short int), 1, file); 
    *codeSegmentContent = (char*) malloc(sizes[2]);
    fread(*codeSegmentContent, sizeof(char), sizes[2], file);
    if(sizes[1]){
        *constSegmentContent = (char*) malloc(sizes[1]);
        fread(*constSegmentContent, sizeof(char), sizes[1], file);    
    }
    fread( &entryPoint, sizeof(short int), 1, file);
}

void vmiVersionOne(FILE* file, char** codeSegmentContent, arguments* args, int sizes[]){
    int* registers = (int*) malloc(32 * sizeof(int)); 
    int* segment_table = (int*) malloc(8 * sizeof(int)); 

    fread(&args->memory_size, sizeof(short int), 1, file); //memory size

    for(int i = 0; i < 32; i++)
        fread(&registers[i], sizeof(int), 1, file); 

    for(int i = 0; i < 8; i++)
        fread(&segment_table[i], sizeof(int), 1, file);

    fread(codeSegmentContent, sizeof(char), args->memory_size, file); //we read the whole memory content here, not just the code segment   

}

void readSizes(FILE* file, int sizes[]) {
    int temp;
    fread(&temp, sizeof(short int), 1, file); //code
    sizes[2] = temp; //&sizes[2]
    fread(&temp, sizeof(short int), 1, file); //data
    sizes[3] = temp;
    fread(&temp, sizeof(short int), 1, file); //extra
    sizes[4] = temp;
    fread(&temp, sizeof(short int), 1, file); //stack
    sizes[5] = temp;
    fread(&temp, sizeof(short int), 1, file); //const
    sizes[1] = temp;
}

//*sizeFile = (unsigned char)title[6] << 8 | (unsigned char)title[7];

void readIdentifier(FILE* file, int* version) { // assumes file exists
    char title[6];

    fread(title, sizeof(char), 6, file);

    if(strncmp(title, "VMX25", 5)!=0 && strncmp(title, "VMI25",5)!=0)
        error_handler.invalidHeader();

    *version = (unsigned char)title[5] - '0';
}
