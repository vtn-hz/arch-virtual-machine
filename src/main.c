#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_handler.h"

#include "virtual_machine.h"

#include "vm_printer.h"
#include "vm_runner.h"

#include "arguments_parsing.h"


void getParsed(char** fileContent, int* sizeFile, char* path);
void readIdentifier(FILE* file, int* version);

int main(int argc, char** argv) {
    char* fileContent;
    arguments args;
    //int sizes[6] = 0; //order: param, const, code, data, extra, stack (same as to be saved in memory)

    getArguments(argc, argv, &args); // code segment size

    printf("Archivo .vmx: %s\n", args.currentVmx);
    printf("Archivo .vmi: %s\n", args.currentVmi);
    printf("Tamaño memoria: %d KB\n", args.memory_size);
    printf("Disasembler: %d\n", args.dissasembler);
    printf("Cantidad de parametros: %d\n", args.paramsAmount);
    for (int i = 0; i < args.paramsAmount; i++)
        printf("Parametro %d: %s\n", i + 1, args.params[i]);
    // getParsed(fileContent,sizeFile,path);

    //VirtualMachine* virtualM = createVm(sizes, args, fileContent);

    /*if (args.dissasembler) 
        virtualMachinePrint(virtualM);
    virtualMachineRun(virtualM);
    
    releaseVm(virtualM);*/
    return 0;
}

/*void getParsed(char** fileContent, int* sizeFile, char* path) {    
    FILE* file = fopen(path, "rb");
    int sizefile;
    
    if (file == NULL)
        error_handler.fileNotFound();

    readIdentifier(file, version);

    if (args->currentVMX){
        if(version == 2){
            readSizes(file, sizes);
            fread((unsigned char)entryPoint, sizeof(char),2,file); 
            *fileContent = (char*) malloc(sizes[2]);
            fread(*fileContent, sizeof(char), sizes[2], file);

        }
        else{
            char tam[2];
            fread(tam, sizeof(char),2,file);
            sizefile = (unsigned char)tam[0] << 8 | (unsigned char)tam[1];
            *fileContent = (char*) malloc(*sizeFile);
            fread(*fileContent, sizeof(char), *sizeFile, file);
        }
    }
    else{

    }

    *fileContent = (char*) malloc(*sizeFile);
    
    fread(*fileContent, sizeof(char), *sizeFile, file);
    fclose(file);
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

    if(strncmp(title, "VMX25", 5)!=0 || strncmp(title, "VMI25",5)!=0)
        error_handler.invalidHeader();

    *version = (unsigned char)title[5] - '0';
}*/