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

VirtualMachine* buildVm(arguments* args, int sizes[]) {
    // name is confuse, maybe sizes couuld be inside args? (initializer package)  mari: i like it, should change it everywhere tho
    VirtualMachine* virtualM = (VirtualMachine*) malloc(sizeof(VirtualMachine));
    
    char* codeSegmentContent;
    int entryPoint = 0;
    char* constSegmentContent;
    int regs[32]; //only for the vmi, kind of wasteful but easier this way
    int segs[8];
    
    getParsed(&codeSegmentContent, &constSegmentContent, args, sizes, &entryPoint, regs, segs);

    if(args->currentVmx){
        virtualM->memory = (unsigned char*) malloc(args->memory_size * 1024);
        createVm(virtualM, sizes, args->memory_size, entryPoint, codeSegmentContent, constSegmentContent, args->params, args->paramsAmount);
    }
    else
        restoreVm(virtualM, args, codeSegmentContent, regs, segs); // change name to restoreVm? mari: like it too, much more precise

    free(codeSegmentContent);

    if (sizes[1])
        free(constSegmentContent); //is it so bad if we do this here? not really, is a local variabe after all

    return virtualM;
}

void createVm(VirtualMachine* virtualM, int sizes[], int memorySize, int entryPoint, char* codeSegmentContent, char* constSegmentContent, char** paramSegmentContent, int paramsAmount) { 
    int reg[8] = {-1, -1, -1, -1, -1, -1, -1, -1};

    if(sizes[0] > 0)
       setParamContentInMemory(virtualM, paramSegmentContent, sizes[0], paramsAmount);

    createSegmentTable(&virtualM->segment_table, memorySize);
    initSegmentTable(&virtualM->segment_table, sizes, reg); 
    setSTRegisters(virtualM, reg, entryPoint, sizes[0] - (paramsAmount*4)); //i send the size of the params minus the size of the pointers

    vmSetUp(virtualM);

    if(sizes[1] > 0)
        setMemoryContent(virtualM, constSegmentContent, sizes[1], virtualM->registers[KS]);

    setMemoryContent(virtualM, codeSegmentContent, sizes[2], virtualM->registers[CS]); // !

    // for (int i = 0; i < 10; i++)
    //     printf("%02X\n", codeSegmentContent[i]);

    // for (int i = 0; i < virtualM->segment_table.descriptors[1].size; i++)
    //     printf("%02X\n", virtualM->memory[i]);
}

void setParamContentInMemory(VirtualMachine* virtualM, char** paramsContent, int paramSegmentSize, int paramsAmount) {
    int pointers[paramsAmount];
    int previousSize = 0;
    int i;
    char* cad;
    char* psContent = (char*) malloc(paramSegmentSize);

    for( i = 0; i < paramsAmount; i++){ //cálculo de los punteros
        pointers[i] = (0x0000 << 16) | previousSize;// whats mean the 0x0000 << 16 ? mari: just to give it the 4 bytes format, kinda ugly tho
        previousSize+=strlen(paramsContent[i]) + 1; //+1 for the null terminator
    }

    int pos = paramsAmount;
    for( i = 0; i < paramsAmount; i++){ //paso los punteros a string (tal vez es innecesario, puede verse)
        cad = intToString(pointers[i]); // change name to intToBytes or something like parseBigEldian ? mari: do you like bigEndianParser?
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

    setMemoryContent(virtualM, psContent, paramSegmentSize, virtualM->registers[PS]);

    free(psContent);
}
    
void restoreVm(VirtualMachine* virtualM, arguments* args, char* fileContent, int regs[], int segs[]) {

    createSegmentTable(&virtualM->segment_table, args->memory_size); //careful here, memory size sent is the default one 'cause we dont have it in vmi files, we calculate it later

    vmSetUp(virtualM);

    for(int i = 0; i < 32; i++)
        virtualM->registers[i] = regs[i];
    
    for(int i = 0; i < DST_MAX; i++){
        virtualM->segment_table.descriptors[i].base = segs[i] >> 16;
        virtualM->segment_table.descriptors[i].size = segs[i] & 0x0000FFFF;
    }

    args->memory_size = (virtualM->segment_table.descriptors[DST_MAX -1].base + virtualM->segment_table.descriptors[DST_MAX -1].size)/1024; // calculate memory size from segments, divide by 1024 to get in KB and follow the format
    virtualM->memory = (unsigned char*) malloc(args->memory_size * 1024);

    setMemoryContent(virtualM, fileContent, args->memory_size * 1024, 0); // esto depende del vmi entonces se le pasa la posición lógica 0 para que inicie la escritura desde el comienzo de la memoria
}

void vmSetUp(VirtualMachine* virtualM) {

    virtualM->mode = GO_MODE; // if it was restored may be initilized with DEBUG_MODE
                              // you could send it as parameter if needed   mari: didnt understand this comment

    initializeGetters();
    initializeSetters();
    initializeInstructions(virtualM);
}

void setSTRegisters(VirtualMachine* virtualM, int reg[], int entrypoint, int paramsSize) {
    int* registers = virtualM->registers;

    registers[PS] = reg[0] == -1 ? reg[0]: reg[0] | paramsSize; //give it the position of the first pointer (i remembered this just now lol)
    registers[KS] = reg[1];
    registers[CS] = reg[2];
    registers[DS] = reg[3];
    registers[ES] = reg[4];
    registers[SS] = reg[5];

    registers[IP] = (registers[CS] & 0xFFFF0000) | (entrypoint & 0x0000FFFF);
    // here we have to set SP to the bottom of the stack segment
    // something like: registers[SP] = (registers[SS] & (virtualM->segment_table[ registers[SS] >> 16 ]).size)  mari: has to be or 'cause it would be all zeros otherwise
    registers[SP] = (registers[SS] | virtualM->segment_table.descriptors[registers[SS] >> 16].size);
    //if(entrypoint)
    //    PUSH( main ret);   dont know how to charge the main ret adress
}

void setMemoryContent(VirtualMachine* virtualM, char* fileContent, int contentSize, int logicalAddress) {
    if (contentSize > DEFAULT_MEMORY_SIZE) {
        error_handler.buildError("Error: el tamaño del contenido {%d} excede la memoria disponible", contentSize);
    }
    int address = transformLogicalAddress(virtualM->segment_table, logicalAddress);
    
    for (int i = address; i < contentSize; i++)
        virtualM->memory[i] = fileContent[i];
}

void releaseVm(VirtualMachine* virtualM) {
    free(virtualM->memory);
    free(virtualM);
}
