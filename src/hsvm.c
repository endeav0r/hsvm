#include <stdio.h>

#include "instruction.h"
#include "vm.h"

int main (int argc, char * argv [])
{
    struct _vm * vm;
    int error;

    vm = vm_load(argv[1]);

    while (1) {
        //printf("%s\n", vm_ins_str(vm));
        error = vm_step(vm);
        if (error) {
            switch (error) {
            case VM_HALTED :
                break;
            case VM_BAD_INSTRUCTION :
                printf("VM Encountered Bad Instruction\n");
                printf("%s\n", vm_registers_str(vm));
                break;
            }
            break;
        }
    }

    vm_destroy(vm);

    return 0;
}