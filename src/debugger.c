#include <stdio.h>
#include <string.h>

#include "instruction.h"
#include "linenoise.h"
#include "vm.h"

#define BREAKPOINTS_SIZE 32
int breakpoints[32];

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


void set_breakpoint (const char * command)
{
    int break_i;
    uint16_t addr;

    if (    (sscanf(command, "b %hx", &addr) == -1)
         && (sscanf(command, "break %hx", &addr) == -1))
        return;

    for (break_i = 0; break_i < BREAKPOINTS_SIZE; break_i++) {
        if (breakpoints[break_i] == -1) {
            breakpoints[break_i] = addr;
            printf("breakpoint set: %hx\n", addr);
            if (break_i < BREAKPOINTS_SIZE - 1)
                breakpoints[break_i + 1] = -1;
            return;
        }
    }

    printf("breakpoint not set, max breakpoints already set\n");
}


void debug_continue (struct _vm * vm)
{
    int break_i;
    int breakpoint_found = 0;

    if (vm->halted) {
        printf("VM Halted\n");
        return;
    }
    while ((vm->halted == 0) && (breakpoint_found == 0)) {
        vm_step(vm);
        for (break_i = 0; break_i < BREAKPOINTS_SIZE; break_i++) {
            if (breakpoints[break_i] == -1)
                break;
            if (breakpoints[break_i] == vm->regs[REG_IP]) {
                printf("breakpoint: %s\n", vm_ins_str(vm));
                breakpoint_found = 1;
                break;
            }
        }
    }
}


void dump_core (struct _vm * vm)
{
    FILE * fh;

    fh = fopen("core", "wb");
    if (fh == NULL) {
        fprintf(stderr, "could not open file core\n");
        return;
    }

    fwrite (vm->mem, 1, 65536, fh);

    fclose(fh);

    printf("core written to file 'core'\n");
}


int main (int argc, char * argv [])
{
    struct _vm * vm;
    char * command;
    char last_command[64];

    breakpoints[0] = -1;

    vm = vm_load(argv[1]);

    printf("%s\n", vm_ins_str(vm));

    while (1) {
        command = linenoise("> ");
        if (strlen(command) == 0)
            command = last_command;
        else {
            linenoiseHistoryAdd(command);
            memset(last_command, 0, 64);
            strncpy(last_command, command, 63);
        }

        if (    (strncmp(command, "break", 5) == 0)
             || (strncmp(command, "b", 1) == 0)) {
            set_breakpoint(command);
        }
        else if (    (strncmp(command, "continue", 5) == 0)
             || (strncmp(command, "c", 1) == 0)) {
            debug_continue(vm);
        }
        else if (strncmp(command, "core", 5) == 0) {
            dump_core(vm);
        }
        else if (    (strncmp(command, "memory", 6) == 0)
             || (strncmp(command, "m", 1) == 0)){
            debug_memory(vm, command);
        }
        else if (    (strcmp(command, "next") == 0)
             || (strcmp(command, "n") == 0)) {
            debug_next(vm);
            printf("%s\n", vm_ins_str(vm));
        }
        else if (    (strcmp(command, "registers") == 0)
                  || (strcmp(command, "r") == 0)) {
            printf("%s\n", vm_registers_str(vm));
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
    }

    vm_destroy(vm);

    return 0;
}