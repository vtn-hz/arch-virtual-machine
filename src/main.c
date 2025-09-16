    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include "error_handler.h"
    #include "segment_table.h"
    #include "virtual_machine.h"

    #include "vm_printer.h"
    #include "vm_runner.h"

    #include "instructions.h"

    // #include "data_access.h"

    void getArguments(int argc, char** argv, char **fileContent, int *sizeFile, int *dissasembler);
    void getParsed(char **fileContent, int *sizeFile, char* path);
    void readIdentifier(FILE *file, int* sizeFile);

    int main(int argc, char** argv) {
        char *fileContent;
        int sizeFile;
        int dissasembler=0;
        
        getArguments(argc, argv, &fileContent, &sizeFile, &dissasembler);

        VirtualMachine* virtualM = createVm(sizeFile,fileContent);
        if (dissasembler) 
            virtualMachinePrint(virtualM);

        virtualMachineRun(virtualM);
        // virtualM->registers[OP1] = 0x031B0003;
        // virtualM->registers[OP2] = 0x02000000 + 'a';
        // virtualM->registers[OPC] = 0x00000010;
        // MOV(virtualM);
        // virtualM->registers[OP1] = 0x031B0002;
        // virtualM->registers[OP2] = 0x02000000 + 'l';
        // virtualM->registers[OPC] = 0x00000010;
        // MOV(virtualM);
        // virtualM->registers[OP1] = 0x031B0001;
        // virtualM->registers[OP2] = 0x02000000 + 'o';
        // virtualM->registers[OPC] = 0x00000010;
        // MOV(virtualM);
        // virtualM->registers[OP1] = 0x031B0000;
        // virtualM->registers[OP2] = 0x02000000 + 'H';
        // virtualM->registers[OPC] = 0x00000010;
        // MOV(virtualM);

        // virtualM->registers[EDX] = virtualM->registers[DS] + 3;
        // virtualM->registers[ECX] = 0x00010004;
        // virtualM->registers[EAX] = 0x001F;
        // virtualM->registers[OP1] = 0x02000002;

        // SYS(virtualM);

        //virtualMachineRun(virtualM);
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

    // Función helper para imprimir resultados de tests
    void printTestResult(const char* testName, int expected, int actual, int passed);

    // Helper para configurar operandos
    void setupOperands(VirtualMachine* vm, int op1_addr, int op1_value, int op2_value);

    // Tests para cada instrucción
    void testSHL(VirtualMachine* vm);
    void testSHR(VirtualMachine* vm);
    void testSAR(VirtualMachine* vm);
    void testAND(VirtualMachine* vm);
    void testOR(VirtualMachine* vm);
    void testXOR(VirtualMachine* vm);
    void testSWAP(VirtualMachine* vm);
    void testLDL(VirtualMachine* vm);
    void testLDH(VirtualMachine* vm);
    void testRND(VirtualMachine* vm);


    /*int main(int argc, char** argv) {
        printf("=== VIRTUAL MACHINE INSTRUCTION TESTS ===\n");
        
        // Crear máquina virtual (simulando el setup que tienes)
        char dummyContent[1000] = {0}; // Contenido dummy para la VM
        VirtualMachine* vm = createVm(1000, dummyContent);
        vmSetUp(vm, 0, 1); // code segment = 0, data segment = 1
        
        // Ejecutar todos los tests usando la misma VM
        testSHL(vm);
        testSHR(vm);
        testSAR(vm);
        testAND(vm);
        testOR(vm);
        testXOR(vm);
        testSWAP(vm);
        testLDL(vm);
        testLDH(vm);
        testRND(vm);
        
        printf("\n=== ALL TESTS COMPLETED ===\n");
        
        // Liberar recursos
        releaseVm(vm);
        
        return 0;
    }*/

    void getArguments(int argc, char** argv,char **fileContent,int *sizeFile,int *dissasembler)
    {
        char *ext;
        //check if file exists
        if(argc<2){ //make new error: arguments missing
            error_handler.fileNotFound();
        }
        
        ext = strrchr(argv[1],'.'); //medio chancho pero funciona
        if(!ext || strcmp(ext,".vmx")!=0){
            error_handler.buildError("Error: el tipo de archivo esperado es .vmx"); //should be invalid file type
        }    
        //check if file is .vmx

        char* path = argv[1];
        //printf("Path: %s\n",path); //debug
        getParsed(fileContent,sizeFile,path);
        
        if(argc>=3)
            *dissasembler = (strcmp(argv[2], "-d")==0);
        // printf("%d",*dissasembler); //debug
    }

    void getParsed(char **fileContent, int *sizeFile, char* path)
    {    
        FILE *file = fopen(path, "rb");
        if(file==NULL){
            error_handler.fileNotFound();
        }
        // check if file exists

        readIdentifier(file,sizeFile);

        // save in memory the content of the file , malloc with the size of the file
        *fileContent = (char*) malloc(*sizeFile);
        
        fread(*fileContent,sizeof(char),*sizeFile,file); 
        //for(int i=0;i<*sizeFile;i++)
        //{
        //    printf("%02X ",(unsigned char)(*fileContent)[i]); //debug
        //}
        fclose(file);
    }

    void readIdentifier(FILE *file,int* sizeFile) //asume que existe archivo
    {
        char title[8];
        //revise formats
        fread(title,sizeof(char),8,file); //works and read what it has to

        (*sizeFile) = ((unsigned char)title[6] <<8) | (unsigned char)title[7]; //save the size
        //printf("%d",sizeFile);
        if(strncmp(title,"VMX25",5))
        {
            error_handler.invalidHeader();
        }
    }

