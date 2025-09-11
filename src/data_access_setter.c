#include <stdlib.h>

#include "data_access.h"

#include "virtual_machine.h"

#include "utils.h"

#include "error_handler.h"

static p_setter_data availableDataSetter[4];

void setData(VirtualMachine*, int operand, int value, int bytes) {

}   

void setDataToMemory(VirtualMachine*, int operand, int value, int bytes) {

}

void initializeSetters () {
    availableDataSetter[3] = setDataToMemory;
}
