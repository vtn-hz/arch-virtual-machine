#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "error_handler.h"
#include "segment_table.h"

void getArguments(int argc, char** argv);
void getParsed(char *fileContent);
void readIdentifier(FILE *file,int* sizeFile);

int main(int argc, char** argv){

    return 0;
}

void getArguments(int argc, char** argv)
{
    char *fileContent;
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

    getParsed(fileContent);
    //call getParsed

    if(argv[2]=="-d")
        dissasembler=true;

    //call segment table with fileContent
}

void getParsed(char *fileContent)
{
    int sizeFile;
    FILE *file = fopen("sample.vmx", "rb");
    if(file==NULL){
        error_handler.fileNotFound();
    }
    // check if file exists

    readIdentifier(file,&sizeFile);

    // save in memory the content of the file , malloc with the size of the file
    fileContent = (char*) malloc(sizeFile * sizeof(char));
    fread(fileContent,sizeof(char),sizeFile,file);

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

