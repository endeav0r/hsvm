#include <stdio.h>

#include "instruction.h"
#include "vm.h"

void core_dump (struct _vm * vm)
{
    FILE * fh;

    fh = fopen("core", "wb");
    if (fh == NULL)
        return;

    fwrite(vm->mem, 1, 65536, fh);

    fclose(fh);
}

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

    core_dump(vm);
    vm_destroy(vm);

    return 0;
}