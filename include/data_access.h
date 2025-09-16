#ifndef DATA_ACCESS_H
#define DATA_ACCESS_H

typedef struct VirtualMachine VirtualMachine;

int  getData(VirtualMachine*, int operand, int bytes);
void setData(VirtualMachine*, int operand, int value, int bytes);

void setDataToMemory(VirtualMachine*, int address, int value, int bytes);
int getDataFromMemory(VirtualMachine*, int address, int bytes);


typedef int(*p_getter_data)(VirtualMachine*, int operand, int bytes);
typedef void(*p_setter_data)(VirtualMachine*, int operand, int value, int bytes);

void initializeGetters();
void initializeSetters();

/*

int main(int argc, char** argv) {
    VirtualMachine* virtual = createVm ( 20 );
    vmSetUp( virtual );

    // 1111 1111 1111 1111 -> -4
    // 000
    
    setData(virtual, 0x0100000A, 777, 3 );
    int value = getData(virtual, 0x0100000A, 3);
    printf("\n\nvalue: %d\n", value);
    releaseVm( virtual );
    return 0;
}

*/
#endif