#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_handler.h"

#include "virtual_machine.h"

#include "vm_printer.h"
#include "vm_runner.h"

void getArguments(int argc, char** argv, char** fileContent, int* sizeFile, int* dissasembler);
void getParsed(char** fileContent, int* sizeFile, char* path);
void readIdentifier(FILE* file, int* sizeFile);

int main(int argc, char** argv) {
    char* fileContent;
    int sizeFile;
    int dissasembler = 0;
    
    getArguments(argc, argv, &fileContent, &sizeFile, &dissasembler);

    VirtualMachine* virtualM = createVm(sizeFile, fileContent);

    if (dissasembler) 
        virtualMachinePrint(virtualM);
    virtualMachineRun(virtualM);
    
    releaseVm(virtualM);
    return 0;
}

void getArguments(int argc, char** argv, char** fileContent, int* sizeFile, int* dissasembler) {
    char* ext;

    // check if input has required arguments
    if (argc < 2)
        error_handler.fileNotFound();
    
    ext = strrchr(argv[1],'.');
    // check if file is .vmx
    if (!ext || strcmp(ext,".vmx") != 0)
        error_handler.buildError("Error: el tipo de archivo esperado es .vmx"); 

    char* path = argv[1];
    getParsed(fileContent,sizeFile,path);
    
    if (argc >= 3)
        *dissasembler = (strcmp(argv[2], "-d") == 0);
}

void getParsed(char** fileContent, int* sizeFile, char* path) {    
    FILE* file = fopen(path, "rb");
    if (file == NULL)
        error_handler.fileNotFound();

    readIdentifier(file, sizeFile);

    *fileContent = (char*) malloc(*sizeFile);
    
    fread(*fileContent, sizeof(char), *sizeFile, file);
    fclose(file);
}

void readIdentifier(FILE* file, int* sizeFile) { // assumes file exists
    char title[8];

    fread(title, sizeof(char), 8, file);

    *sizeFile = (unsigned char)title[6] << 8 | (unsigned char)title[7];

    if(strncmp(title, "VMX25", 5))
        error_handler.invalidHeader();
}