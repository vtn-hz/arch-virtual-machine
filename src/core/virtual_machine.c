#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "virtual_machine.h"

#include "vm_mode.h"

#include "segment_table.h"

#include "data_access.h"

#include "error_handler.h"

#include "arguments_parsing.h"

#include "files_parsing.h"

#include "utils.h"

void initializeStack( VirtualMachine* vm, int entryPoint ) ;

VirtualMachine* buildVm(arguments* args, int sizes[]) {
    // name is confuse, maybe sizes couuld be inside args? (initializer package)  mari: i like it, should change it everywhere tho
    VirtualMachine* virtualM = (VirtualMachine*) malloc(sizeof(VirtualMachine));
    
    char* codeSegmentContent;
    int entryPoint = 0;
    char* constSegmentContent;
    int regs[32]; //only for the vmi, kind of wasteful but easier this way
    int segs[8];
    
    getParsed(&codeSegmentContent, &constSegmentContent, args, sizes, &entryPoint, regs, segs);
    // printf("VMX: %s", args->currentVmx);

    // printf("vuelve de getparsed \n");
    if(args->currentVmx){        
        virtualM->memory = (unsigned char*) malloc(args->memory_size * 1024);
        createVm(virtualM, sizes, args->memory_size, entryPoint, codeSegmentContent, constSegmentContent, args->params, args->paramsAmount);
    }
    else{
        // printf("Program it was dentified to be a vmi\n");
        restoreVm(virtualM, args, codeSegmentContent, regs, segs); 
    }

    free(codeSegmentContent);

    if (sizes[1])
        free(constSegmentContent); //is it so bad if we do this here? not really, is a local variabe after all

    return virtualM;
}

void createVm(VirtualMachine* vm, int sizes[], int memorySize, int entryPoint, char* codeSegmentContent, char* constSegmentContent, char** paramSegmentContent, int paramsAmount) { 
    int reg[8] = {-1, -1, -1, -1, -1, -1, -1, -1};

    if(sizes[0] > 0)
       setParamContentInMemory(vm, memorySize, paramSegmentContent, sizes[0], paramsAmount);

    createSegmentTable(&vm->segment_table, memorySize);
    initSegmentTable(&vm->segment_table, sizes, reg); 
    setSTRegisters(vm, reg, entryPoint, sizes[0] - (paramsAmount*4)); //i send the size of the params minus the size of the pointers

    vmSetUp(vm, GO_MODE);

    if(sizes[1] > 0)
        setMemoryContent(vm, memorySize, constSegmentContent, sizes[1], vm->registers[KS]);
    setMemoryContent(vm, memorySize, codeSegmentContent, sizes[2], vm->registers[CS]); // !


    printf("VM Created Successfully!\n");
    // printf("Memory Dump:\n");
    int total = vm->segment_table.descriptors[ vm->segment_table.counter-1 ].base +
                vm->segment_table.descriptors[ vm->segment_table.counter-1 ].size;
    // for (int i = 0; i < 100; i++)
    //     printf("%02X ", vm->memory[i]);

    // printf("\nSegment Table:\n");
    // for (int i = 0; i < 8; i++)
    //     printf("%-8d | %-8d\n", vm->segment_table.descriptors[i].base, vm->segment_table.descriptors[i].size);

    initializeStack( vm, entryPoint);
}

void initializeStack( VirtualMachine* vm, int entryPoint ) {
    if (entryPoint == 0) 
        return;

    if (vm->registers[SS] == -1) 
        return;


    vm->registers[SP] -= 4;
    prepareSetMemoryAccess(vm, SP, 0, -1, DEFAULT_ACCESS_SIZE);
    commitSetMemoryAccess(vm);
}


void setParamContentInMemory(VirtualMachine* virtualM, int memorySize, char** paramsContent, int paramSegmentSize, int paramsAmount) {
    return;
    int pointers[paramsAmount];
    int previousSize = 0;
    int i;

    char cad[4];
    char* psContent = (char*) malloc(paramSegmentSize);

    for( i = 0; i < paramsAmount; i++){ //cálculo de los punteros
        pointers[i] = (0x0000 << 16) | previousSize;// whats mean the 0x0000 << 16 ? mari: just to give it the 4 bytes format, kinda ugly tho
        previousSize += strlen(paramsContent[i]) + 1; //+1 for the null terminator
    }

    int pos = paramsAmount;
    for( i = 0; i < paramsAmount; i++){ // paso los punteros a string (tal vez es innecesario, puede verse)
        toBigEndian(cad, pointers[i], 4);
        paramsContent[pos] = cad;
        pos++;
    }

    int offset = 0;
    for (i = 0; i < paramsAmount; i++) { //es paramsamount porque es la cantidad de strings
        strcpy(psContent + offset, paramsContent[i]);
        offset += strlen(paramsContent[i]) + 1; // +1 por el '\0'
    }

    for (int i = paramsAmount; i < pos; i++) //vacio la memoria que utilicé para los punteros
        free(paramsContent[i]);

    setMemoryContent(virtualM, memorySize, psContent, paramSegmentSize, virtualM->registers[PS]);

    free(psContent);
}
    
void restoreVm(VirtualMachine* virtualM, arguments* args, char* fileContent, int regs[], int segs[]) {

    createSegmentTable(&virtualM->segment_table, args->memory_size); //careful here, memory size sent is the default one 'cause we dont have it in vmi files, we calculate it later
    // printf("Segment table created succesfully\n");
    vmSetUp(virtualM, DEBUG_MODE);

    for(int i = 0; i < 32; i++)
        virtualM->registers[i] = regs[i];
    
    for(int i = 0; i < DST_MAX; i++){
        if(segs[i]!= -1){
            addSegment(&virtualM->segment_table, segs[i] & 0xFFFF);
        }
    }

    args->memory_size = (virtualM->segment_table.descriptors[6-1].base + virtualM->segment_table.descriptors[6-1].size)/1024; // calculate memory size from segments, divide by 1024 to get in KB and follow the format
    virtualM->memory = (unsigned char*) malloc(args->memory_size * 1024);

    //setMemoryContent(virtualM, args->memory_size*1024, fileContent, args->memory_size * 1024, 0); // esto depende del vmi entonces se le pasa la posición lógica 0 para que inicie la escritura desde el comienzo de la memoria
    for (int i = 0; i < args->memory_size*1024; i++)
        virtualM->memory[i] = fileContent[i];  
}

void vmSetUp(VirtualMachine* virtualM, char mode) {

    virtualM->mode = mode; // if it was restored may be initilized with DEBUG_MODE
                              // you could send it as parameter if needed   mari: didnt understand this comment

    initializeGetters();
    initializeSetters();
    initializeInstructions(virtualM);
}

void setSTRegisters(VirtualMachine* virtualM, int reg[], int entryPoint, int paramsSize) {
    int* registers = virtualM->registers;

    registers[PS] = reg[0] == -1 ? reg[0]: reg[0] | paramsSize; //give it the position of the first pointer (i remembered this just now lol)
    registers[KS] = reg[1];
    registers[CS] = reg[2];
    registers[DS] = reg[3];
    registers[ES] = reg[4];
    registers[SS] = reg[5];

    registers[IP] = (registers[CS] & 0xFFFF0000) | (entryPoint & 0x0000FFFF);
    // here we have to set SP to the bottom of the stack segment
    // something like: registers[SP] = (registers[SS] & (virtualM->segment_table[ registers[SS] >> 16 ]).size)  mari: has to be or 'cause it would be all zeros otherwise
    registers[SP] = (registers[SS] | virtualM->segment_table.descriptors[registers[SS] >> 16].size);
    if(entryPoint != 0) {
        //  dont know how to charge the main ret adress // i know how 😎
        
    }
}

void setMemoryContent(VirtualMachine* virtualM, int memorySize, unsigned char* fileContent, int contentSize, int logicalAddress) {
    // printf("Memory: %d\n", memorySize);
    if (contentSize > memorySize * 1024) 
        error_handler.buildError("Error: el tamaño del contenido {%d} excede la memoria disponible", contentSize);

    int address = transformLogicalAddress(virtualM->segment_table, logicalAddress);
    for (int i = address, j=0; j < contentSize; i++, j++)
        virtualM->memory[i] = fileContent[j];  
}

void releaseVm(VirtualMachine* virtualM) {
    free(virtualM->memory);
    free(virtualM);
}
