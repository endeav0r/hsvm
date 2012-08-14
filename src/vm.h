#ifndef vm_HEADER
#define vm_HEADER

#include <inttypes.h>

#define VM_MEMSIZE 65536

#define VM_OK              0
#define VM_HALTED          1
#define VM_BAD_INSTRUCTION 2

#define VM_SYSCALL_OPEN  0
#define VM_SYSCALL_READ  1
#define VM_SYSCALL_WRITE 2
#define VM_SYSCALL_CLOSE 3

#define VM_OPEN_READ   0x1
#define VM_OPEN_WRITE  0x2
#define VM_OPEN_APPEND 0x4
#define VM_OPEN_CREATE 0x8

struct _vm {
	unsigned char * mem;
	uint16_t regs [11];
	int flags;
	int halted;
	char debug_str[256];
};

struct _vm * vm_create  (unsigned char * mem);
struct _vm * vm_load    (const char * filename);
void         vm_destroy (struct _vm *);
void         vm_syscall (struct _vm *);

void         vm_syscall_open  (struct _vm *);
void         vm_syscall_read  (struct _vm *);
void         vm_syscall_write (struct _vm *);
void         vm_syscall_close (struct _vm *);

int          vm_step          (struct _vm *);
const char * vm_registers_str (struct _vm *);
const char * vm_ins_str       (struct _vm *);
uint8_t      vm_opcode        (struct _vm *);

#endif