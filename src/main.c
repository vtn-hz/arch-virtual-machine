#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "error_handler.h"
#include "segment_table.h"
#include "virtual_machine.h"

void getArguments(int argc, char** argv,char *fileContent,int *sizeFile);
void getParsed(char *fileContent,int *sizeFile);
void readIdentifier(FILE *file,int* sizeFile);

int main(int argc, char** argv) {
    char *fileContent;
    int sizeFile;
    getArguments(argc, argv,fileContent,&sizeFile);
    VirtualMachine* virtualM = createVm(sizeFile,fileContent);
    //vmSetUp( virtual );
    //addSegment(&virtual->segment_table, 0xA);
    //addSegment(&virtual->segment_table, 200);
    //printf("Memory left: %d\n", memorySizeLeft(virtual->segment_table));
    //printf("Base CS: %d \n",virtual->segment_table.descriptors[0].base);
    //printf("Base DS: %d \n",virtual->segment_table.descriptors[1].base);
    //printf("Transform Logical Address 0x010A to Physical Address: %d \n", transformLogicalAddress(virtual->segment_table, 0x0001000A));
    //printf(isLogicalAddressValid(virtual->segment_table, 0x0000000A) ? "Valid Address\n" : "Invalid Address\n");
    releaseVm(virtualM);
    return 0;
}

void getArguments(int argc, char** argv,char *fileContent,int *sizeFile)
{
    char *ext;
    bool dissasembler=false;
    //check if file exists
    if(argc<2){ //make new error: arguments missing
        error_handler.fileNotFound();
    }
    
    ext = strrchr(argv[1],'.'); //medio chancho pero funciona
    if(!ext || strcmp(ext,".vmx")!=0){
        error_handler.invalidInstruction(); //should be invalid file type
    }    
    //check if file is .vmx

    getParsed(fileContent,sizeFile);
    //call getParsed

    if(argv[2]=="-d")
        dissasembler=true;

    //call segment table with fileContent
}

void getParsed(char *fileContent,int *sizeFile)
{
    FILE *file = fopen("sample.vmx", "rb");
    if(file==NULL){
        error_handler.fileNotFound();
    }
    // check if file exists

    readIdentifier(file,sizeFile);

    // save in memory the content of the file , malloc with the size of the file
    *fileContent = (char*) malloc(*sizeFile * sizeof(char));
    fread(*fileContent,sizeof(char),*sizeFile,file); //shall we check if it read everything?

    fclose(file);
}

void readIdentifier(FILE *file,int* sizeFile) //asume que existe archivo
{
    char title[8];
     //revise formats
    fread(title,sizeof(char),8,file); //works and read what it has to
    printf("%s",title); 

    (*sizeFile) = ((unsigned char)title[6] <<8) | (unsigned char)title[7]; //save the size
    //printf("%d",sizeFile);
    if( !strncmp(title,"VMX25",5))
    {
        error_handler.invalidHeader();
    }
}

