#include "vm.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef WINDOWS
    #include <winsock2.h>
#else
    #include <arpa/inet.h>
#endif

#include "instruction.h"

//#define DEBUG_SYSCALL

struct _vm * vm_create (unsigned char * mem)
{
    struct _vm * vm;
    int i;

    vm = (struct _vm *) malloc(sizeof(struct _vm));
    vm->mem = (unsigned char *) malloc(VM_MEMSIZE);
    memcpy(vm->mem, mem, VM_MEMSIZE);

    for (i = 0; i < 11; i++) vm->regs[i] = 0;
    vm->regs[REG_IP] = 0;
    vm->regs[REG_SP] = 0xfff8;
    vm->flags = 0;
    vm->halted = 0;

    return vm;
}


struct _vm * vm_load (const char * filename)
{
    unsigned char * mem;
    FILE * fh;
    size_t filesize;
    struct _vm * vm;

    fh = fopen(filename, "rb");
    if (fh == NULL)
        return NULL;

    fseek(fh, 0, SEEK_END);
    filesize = ftell(fh);
    fseek(fh, 0, SEEK_SET);

    mem = (unsigned char *) malloc(VM_MEMSIZE);
    memset(mem, 0, VM_MEMSIZE);

    fread(mem, 1, filesize, fh);

    fclose(fh);

    vm = vm_create(mem);

    free(mem);

    return vm;
}


void vm_destroy (struct _vm * vm)
{
    free(vm->mem);
    free(vm);
}


void vm_syscall (struct _vm * vm)
{
    switch (vm->regs[REG_0]) {
    case VM_SYSCALL_OPEN  : vm_syscall_open(vm);  break;
    case VM_SYSCALL_READ  : vm_syscall_read(vm);  break;
    case VM_SYSCALL_WRITE : vm_syscall_write(vm); break;
    case VM_SYSCALL_CLOSE : vm_syscall_close(vm); break;
    }
}


void vm_syscall_open (struct _vm * vm)
{
    unsigned int flags = 0;
    uint16_t r1 = vm->regs[REG_1];
    uint16_t r2 = vm->regs[REG_2];
    uint16_t r3 = vm->regs[REG_3];
    char * filename = (char *) &(vm->mem[r1]);

    if (r2 & VM_OPEN_READ)
        flags |= O_RDONLY;
    if (r2 & VM_OPEN_WRITE)
        flags |= O_WRONLY;
    if (r2 & VM_OPEN_APPEND)
        flags |= O_APPEND;
    if (r2 & VM_OPEN_CREATE)
        flags |= O_CREAT;

    #ifdef DEBUG_SYSCALL
    printf("syscall open: (%x) %s %x %x\n", r1, filename, r2, r3);
    #endif

    vm->regs[REG_0] = open(filename, flags, r3);
}


void vm_syscall_read (struct _vm * vm)
{
    uint16_t r1 = vm->regs[REG_1];
    uint16_t r2 = vm->regs[REG_2];
    uint16_t r3 = vm->regs[REG_3];
    void * buf  = &(vm->mem[r2]);

    #ifdef DEBUG_SYSCALL
    printf("syscall read: %x %x %x\n", r1, r2, r3);
    #endif

    vm->regs[REG_0] = read(r1, buf, r3);
}


void vm_syscall_write (struct _vm * vm)
{
    uint16_t r1 = vm->regs[REG_1];
    uint16_t r2 = vm->regs[REG_2];
    uint16_t r3 = vm->regs[REG_3];
    void * buf  = &(vm->mem[r2]);

    vm->regs[REG_0] = write(r1, buf, r3);
}


void vm_syscall_close (struct _vm * vm)
{
    uint16_t r1 = vm->regs[REG_1];

    vm->regs[REG_0] = close(r1);
}


const char * vm_registers_str (struct _vm * vm)
{
    snprintf(vm->debug_str, 256,
             "r0=%04x r1=%04x r2=%04x r3=%04x r4=%04x r5=%04x\n"
             "r6=%04x r7=%04x rsp=%04x rbp=%04x\n"
             "rip=%04x flags=%d",
             vm->regs[REG_0], vm->regs[REG_1], vm->regs[REG_2],
             vm->regs[REG_3], vm->regs[REG_4], vm->regs[REG_5],
             vm->regs[REG_6], vm->regs[REG_7], vm->regs[REG_SP],
             vm->regs[REG_BP], vm->regs[REG_IP], vm->flags);
    return vm->debug_str;
}


const char * vm_ins_str (struct _vm * vm)
{
    char tmp[256];
    struct _instruction * ins;
    ins = (struct _instruction *) &(vm->mem[vm->regs[REG_IP]]);
    ins_str(ins, tmp, 256);
    snprintf(vm->debug_str, 256, "%04x %s", vm->regs[REG_IP], tmp);
    return vm->debug_str;
}


uint8_t vm_opcode (struct _vm * vm)
{
    struct _instruction * ins;
    ins = (struct _instruction *) &(vm->mem[vm->regs[REG_IP]]);
    return ins->opcode;
}


int vm_step (struct _vm * vm)
{
    const struct _instruction * ins;
    uint16_t tmp;

    if (vm->halted) return VM_HALTED;

    ins = (const struct _instruction *) &(vm->mem[vm->regs[REG_IP]]);

    vm->regs[REG_IP] += 4;

    switch (ins->opcode) {
    case OP_ADD :
        vm->regs[ins->operand_0] = vm->regs[ins->operand_1] + vm->regs[ins->operand_2];
        break;
    case OP_SUB :
        vm->regs[ins->operand_0] = vm->regs[ins->operand_1] - vm->regs[ins->operand_2];
        break;
    case OP_MUL :
        vm->regs[ins->operand_0] = vm->regs[ins->operand_1] * vm->regs[ins->operand_2];
        break;
    case OP_DIV :
        vm->regs[ins->operand_0] = vm->regs[ins->operand_1] / vm->regs[ins->operand_2];
        break;
    case OP_MOD :
        vm->regs[ins->operand_0] = vm->regs[ins->operand_1] % vm->regs[ins->operand_2];
        break;
    case OP_AND :
        vm->regs[ins->operand_0] = vm->regs[ins->operand_1] & vm->regs[ins->operand_2];
        break;
    case OP_OR :
        vm->regs[ins->operand_0] = vm->regs[ins->operand_1] | vm->regs[ins->operand_2];
        break;
    case OP_XOR :
        vm->regs[ins->operand_0] = vm->regs[ins->operand_1] ^ vm->regs[ins->operand_2];
        break;
    case OP_ADDLVAL :
        vm->regs[ins->operand_0] += ntohs(ins->lval);
        break;
    case OP_SUBLVAL :
        vm->regs[ins->operand_0] -= ntohs(ins->lval);
        break;
    case OP_MULLVAL :
        vm->regs[ins->operand_0] *= ntohs(ins->lval);
        break;
    case OP_DIVLVAL :
        vm->regs[ins->operand_0] /= ntohs(ins->lval);
        break;
    case OP_MODLVAL :
        vm->regs[ins->operand_0] %= ntohs(ins->lval);
        break;
    case OP_ANDLVAL :
        vm->regs[ins->operand_0] &= ntohs(ins->lval);
        break;
    case OP_ORLVAL :
        vm->regs[ins->operand_0] |= ntohs(ins->lval);
        break;
    case OP_XORLVAL :
        vm->regs[ins->operand_0] ^= ntohs(ins->lval);
        break;
    case OP_JMP :
        vm->regs[REG_IP] += ntohs(ins->lval);
        break;
    case OP_JE :
        if (vm->flags == 0)
            vm->regs[REG_IP] += ntohs(ins->lval);
        break;
    case OP_JNE :
        if (vm->flags != 0)
            vm->regs[REG_IP] += ntohs(ins->lval);
        break;
    case OP_JL :
        if (vm->flags < 0)
            vm->regs[REG_IP] += ntohs(ins->lval);
        break;
    case OP_JLE :
        if (vm->flags <= 0)
            vm->regs[REG_IP] += ntohs(ins->lval);
        break;
    case OP_JG :
        if (vm->flags > 0)
            vm->regs[REG_IP] += ntohs(ins->lval);
        break;
    case OP_JGE :
        if (vm->flags >= 0)
            vm->regs[REG_IP] += ntohs(ins->lval);
        break;
    case OP_CALL  :
    case OP_CALLR :
        vm->regs[REG_SP] -= 2;
        tmp = htons(vm->regs[REG_IP]);
        memcpy(&(vm->mem[vm->regs[REG_SP]]), &tmp, 2);
        if (ins->opcode == OP_CALL)
            vm->regs[REG_IP] += ntohs(ins->lval);
        else
            vm->regs[REG_IP] += vm->regs[ins->operand_0];
        break;
    case OP_RET :
        memcpy(&tmp, &(vm->mem[vm->regs[REG_SP]]), 2);
        vm->regs[REG_IP] = ntohs(tmp);
        vm->regs[REG_SP] += 2;
        break;
    case OP_LOAD :
        memcpy(&tmp, &(vm->mem[ntohs(ins->lval)]), 2);
        vm->regs[ins->operand_0] = ntohs(tmp);
        break;
    case OP_LOADR :
        memcpy(&tmp, &(vm->mem[vm->regs[ins->operand_1]]), 2);
        vm->regs[ins->operand_0] = ntohs(tmp);
        break;
    case OP_LOADB :
        vm->regs[ins->operand_0] = vm->mem[ntohs(ins->lval)];
        break;
    case OP_LOADBR :
        vm->regs[ins->operand_0] = vm->mem[vm->regs[ins->operand_1]];
        break;
    case OP_STOR :
        tmp = htons(vm->regs[ins->operand_0]);
        memcpy(&(vm->mem[ntohs(ins->lval)]), &tmp, 2);
        break;
    case OP_STORR :
        tmp = htons(vm->regs[ins->operand_1]);
        memcpy(&(vm->mem[vm->regs[ins->operand_0]]), &tmp, 2);
        break;
    case OP_STORB :
        vm->mem[ins->lval] = vm->regs[ins->operand_0] & 0xff;
        break;
    case OP_STORBR :
        vm->mem[vm->regs[ins->operand_0]] = vm->regs[ins->operand_1] & 0xff;
        break;
    case OP_IN :
        vm->regs[ins->operand_0] = getchar();
        break;
    case OP_OUT :
        putchar(vm->regs[ins->operand_0]);
        fflush(stdout);
        break;
    case OP_PUSH :
        vm->regs[REG_SP] -= 2;
        tmp = htons(vm->regs[ins->operand_0]);
        memcpy(&(vm->mem[vm->regs[REG_SP]]), &tmp, 2);
        break;
    case OP_PUSHLVAL :
        vm->regs[REG_SP] -= 2;
        tmp = ins->lval; // already in correct byte order
        memcpy(&(vm->mem[vm->regs[REG_SP]]), &tmp, 2);
        break;
    case OP_POP :
        memcpy(&tmp, &(vm->mem[vm->regs[REG_SP]]), 2);
        vm->regs[ins->operand_0] = ntohs(tmp);
        vm->regs[REG_SP] += 2;
        break;
    case OP_MOV :
        vm->regs[ins->operand_0] = vm->regs[ins->operand_1];
        break;
    case OP_MOVLVAL :
        vm->regs[ins->operand_0] = ntohs(ins->lval);
        break;
    case OP_CMP :
        vm->flags = vm->regs[ins->operand_0] - vm->regs[ins->operand_1];
        break;
    case OP_CMPLVAL :
        vm->flags = vm->regs[ins->operand_0] - ntohs(ins->lval);
        break;
    case OP_HLT :
        vm->halted = 1;
        return VM_HALTED;
    case OP_SYSCALL :
        vm_syscall(vm);
        break;
    case OP_NOP :
        break;
    default :
        // on a bad instruction, reset IP
        vm->regs[REG_IP] -= 4;
        vm->halted = 1;
        return VM_BAD_INSTRUCTION;
    }
    return 0;
}