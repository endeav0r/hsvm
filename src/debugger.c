#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "instruction.h"
#include "vm.h"

int debug_next (struct _vm * vm)
{
    int error = vm_step(vm);

    switch (error) {
    case VM_HALTED :
        printf("VM Halted\n");
        break;
    case VM_BAD_INSTRUCTION :
        printf("VM Encountered Bad Instruction\n");
        break;
    }

    return error;
}

int debug_step (struct _vm * vm)
{
    int depth = 0;
    int error = 0;

    if (vm_opcode(vm) == OP_CALL)
        depth = 1;
    else
        return debug_next(vm);

    while (depth > 0) {
        error = debug_next(vm);
        if (error)
            return error;
        if (vm_opcode(vm) == OP_CALL)
            depth++;
        else if (vm_opcode(vm) == OP_RET)
            depth--;
    }

    return debug_next(vm);
}

int debug_memory (struct _vm * vm, const char * command)
{
    int address = vm->regs[REG_SP];
    int length  = 8;
    int matched;
    int i;

    matched = sscanf(command, "m %x %d", &address, &length);
    if (matched == -1) {
        matched = sscanf(command, "memory %x %d", &address, &length);
    }

    printf("%04x: ", address);
    for (i = 0; i < length; i++) {
        if (address + i >= 0x10000)
            break;
        printf("%02x", vm->mem[address + i]);
        if (((i + 1) % 16 == 0) && (i + 1 < length))
            printf("\n%04x: ", address + i + 1);
        else if ((i + 1) % 8 == 0)
            printf("  ");
        else if ((i + 1) % 2 == 0)
            printf(" ");
    }

    printf("\n");

    return 0;
}

int main (int argc, char * argv [])
{
	struct _vm * vm;
    char * command;
    char last_command[64];

	vm = vm_load(argv[1]);

    printf("%s\n", vm_ins_str(vm));

	while (1) {
        command = readline("> ");
        if (strlen(command) == 0)
            command = last_command;
        else {
            add_history(command);
            memset(last_command, 0, 64);
            strncpy(last_command, command, 63);
        }

        if (    (strcmp(command, "next") == 0)
             || (strcmp(command, "n") == 0)) {
            debug_next(vm);
            printf("%s\n", vm_ins_str(vm));
        }
        else if (strcmp(command, "restart") == 0) {
            vm_destroy(vm);
            vm = vm_load(argv[1]);
        }
        else if (    (strcmp(command, "step") == 0)
                  || (strcmp(command, "s") == 0)) {
            debug_step(vm);
            printf("%s\n", vm_ins_str(vm));
        }
        else if (    (strcmp(command, "registers") == 0)
                  || (strcmp(command, "r") == 0)) {
            printf("%s\n", vm_registers_str(vm));
        }
        else if (    (strncmp(command, "memory", 6) == 0)
                  || (strncmp(command, "m", 1) == 0)){
            debug_memory(vm, command);
        }
    }

    vm_destroy(vm);

    return 0;
}