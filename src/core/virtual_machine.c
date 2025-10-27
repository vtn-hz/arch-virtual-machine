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

void initializeStack( VirtualMachine* vm, int entryPoint, int argc, int argv ); ;

VirtualMachine* buildVm(arguments* args, int sizes[]) {
    // name is confuse, maybe sizes couuld be inside args? (initializer package)  mari: i like it, should change it everywhere tho
    VirtualMachine* virtualM = (VirtualMachine*) malloc(sizeof(VirtualMachine));
    
    char* codeSegmentContent;
    int entryPoint = -1;
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

    // printf("entrypoint: %08X \n",entryPoint);
    // printf("esta en crear la vm\n");
    // printf("cantidad de parametros: %d\n",paramsAmount);
    // printf("parametro: %s \n",paramSegmentContent[0]);

    createSegmentTable(&vm->segment_table, memorySize);
    initSegmentTable(&vm->segment_table, sizes, reg); 
    setSTRegisters(vm, reg, entryPoint, sizes[0] - (paramsAmount*4)); //i send the size of the params minus the size of the pointers
    
    if(sizes[0] > 0)
        setParamContentInMemory(vm, memorySize, paramSegmentContent, sizes[0], paramsAmount);

    vmSetUp(vm, GO_MODE);

    if(sizes[1] > 0)
        setMemoryContent(vm, memorySize, constSegmentContent, sizes[1], vm->registers[KS]);
    setMemoryContent(vm, memorySize, codeSegmentContent, sizes[2], vm->registers[CS]); // !

    // printf("Memory Dump:\n");
    int total = vm->segment_table.descriptors[ vm->segment_table.counter-1 ].base +
                vm->segment_table.descriptors[ vm->segment_table.counter-1 ].size;
    // for (int i = 0; i < 100; i++)
    //     printf("%02X ", vm->memory[i]);

    // printf("\nSegment Table:\n");
    // for (int i = 0; i < 8; i++)
    //     printf("%-8d | %-8d\n", vm->segment_table.descriptors[i].base, vm->segment_table.descriptors[i].size);

    initializeStack( vm, entryPoint, paramsAmount, sizes[0] > 0 ? (sizes[0] - (paramsAmount*4)) : -1);
}

void initializeStack( VirtualMachine* vm, int entryPoint, int argc, int argv ) {
    if (entryPoint == -1) 
        return;

    if (vm->registers[SS] == -1) 
        return;

    executeDataPush(vm, argv); // vm->registers[PS] could be sent instead of argv
    executeDataPush(vm, argc);
    executeDataPush(vm, -1);
}


void setParamContentInMemory(VirtualMachine* virtualM, int memorySize, char** paramsContent, int paramSegmentSize, int paramsAmount) {
    int pointers[paramsAmount];
    int previousSize = 0;
    int i;

    char cad[4];
    char* psContent = (char*) malloc(paramSegmentSize);

    for( i = 0; i < paramsAmount; i++){ //cálculo de los punteros
        pointers[i] = (0x0000 << 16) | previousSize;
        previousSize += strlen(paramsContent[i]) + 1; //+1 for the null terminator
    }

    int pos = paramsAmount;
    for( i = 0; i < paramsAmount; i++){ // paso los punteros a string (tal vez es innecesario, puede verse)
        toBigEndian(cad, pointers[i], 4);
        printf("entra al for \n");
        paramsContent[pos] = malloc (4);
        for( int j = 0; j < 4; j++)
            paramsContent[pos][j] = cad[j];
        pos++;
    }
    
    size_t offset = 0;
    // for (i = 0; i < paramsAmount; i++) { //es paramsamount porque es la cantidad de strings
    //     printf("en paramsContent: %s \n",paramsContent[i]);
    //     strcpy(psContent + offset, paramsContent[i]);
    //     printf("en psContent: %s \n",psContent[i]);
    //     offset += strlen(paramsContent[i]) + 1; // +1 por el '\0'
    // }    

    // Para los primeros strings:
    for (i = 0; i < paramsAmount; i++) {
        size_t len = strlen(paramsContent[i]) + 1;
        memcpy(psContent + offset, paramsContent[i], len);
        offset += len;
    }

    // Para los punteros en binario (4 bytes c/u)
    for (i = paramsAmount; i < pos; i++) {
        memcpy(psContent + offset, paramsContent[i], 4);
        offset += 4;
    }

    setMemoryContent(virtualM, memorySize, psContent, paramSegmentSize, 0);

    free(psContent);
}
    
void restoreVm(VirtualMachine* vm, arguments* args, char* fileContent, int regs[], int segs[]) {

    createSegmentTable(&vm->segment_table, args->memory_size); //careful here, memory size sent is the default one 'cause we dont have it in vmi files, we calculate it later
    // printf("Segment table created succesfully\n");
    vmSetUp(vm, DEBUG_MODE);

    for(int i = 0; i < 32; i++)
        vm->registers[i] = regs[i];
    
    for(int i = 0; i < DST_MAX; i++)
        if(segs[i]!= -1)
            addSegment(&vm->segment_table, segs[i] & 0xFFFF);

    int mem =  
        vm->segment_table.descriptors[vm->segment_table.counter - 1].base + 
        vm->segment_table.descriptors[vm->segment_table.counter - 1].size; // calculate memory size from segments, divide by 1024 to get in KB and follow the format
    args->memory_size = 16;
    vm->memory = (unsigned char*) malloc(args->memory_size * 1024);

    for (int i = 0; i < args->memory_size*1024; i++)
        vm->memory[i] = fileContent[i];  
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

    entryPoint = entryPoint == -1 ? 0 : entryPoint;
    registers[IP] = (registers[CS] & 0xFFFF0000) | (entryPoint & 0x0000FFFF);
    // here we have to set SP to the bottom of the stack segment
    // something like: registers[SP] = (registers[SS] & (virtualM->segment_table[ registers[SS] >> 16 ]).size)  mari: has to be or 'cause it would be all zeros otherwise
    registers[SP] = (registers[SS] | virtualM->segment_table.descriptors[registers[SS] >> 16].size);
}

void setMemoryContent(VirtualMachine* virtualM, int memorySize, unsigned char* fileContent, int contentSize, int logicalAddress) {
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
