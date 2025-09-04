#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){

    FILE *file= fopen("sample.vmx","rb");
    readIdentifier(file);
    return 0;
}

void getParsed()
{
}

void readIdentifier(FILE *file) //asume que existe archivo
{
    int sizeFile;
    char title[8];
    file = fopen("sample.vmx", "rb"); //revise formats
    fread(title,sizeof(char),8,file); //works and read what it has to
    printf("%s",title); 

    sizeFile = ((unsigned char)title[6] <<8) | (unsigned char)title[7]; //save the size
    printf("%d",sizeFile);

    if( !strcmp(title,"VMX25"))
        errorHandler; //array to functions for errors
}


