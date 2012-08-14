#ifndef instruction_HEADER
#define instruction_HEADER

#include <inttypes.h>
#include <stdio.h>

#define REG_0   0x00
#define REG_1   0x01
#define REG_2   0x02
#define REG_3   0x03
#define REG_4   0x04
#define REG_5   0x05
#define REG_6   0x06
#define REG_7   0x0A
#define REG_BP  0x08
#define REG_SP  0x09
#define REG_IP  0x07

#define OP_ADD      0x10
#define OP_ADDLVAL  0x11
#define OP_SUB      0x12
#define OP_SUBLVAL  0x13
#define OP_MUL      0x14
#define OP_MULLVAL  0x15
#define OP_DIV      0x16
#define OP_DIVLVAL  0x17
#define OP_MOD      0x18
#define OP_MODLVAL  0x19
#define OP_AND      0x1A
#define OP_ANDLVAL  0x1B
#define OP_OR       0x1C
#define OP_ORLVAL   0x1D
#define OP_XOR      0x1E
#define OP_XORLVAL  0x1F
#define OP_JMP      0x20
#define OP_JE       0x21
#define OP_JNE      0x22
#define OP_JL       0x23
#define OP_JLE      0x24
#define OP_JG       0x25
#define OP_JGE      0x26
#define OP_CALL     0x27
#define OP_CALLR    0x28
#define OP_RET      0x29
#define OP_LOAD     0x30
#define OP_LOADR    0x31
#define OP_LOADB    0x32
#define OP_LOADBR   0x33
#define OP_STOR     0x34
#define OP_STORR    0x35
#define OP_STORB    0x36
#define OP_STORBR   0x37
#define OP_IN       0x40
#define OP_OUT      0x41
#define OP_PUSH     0x42
#define OP_PUSHLVAL 0x43
#define OP_POP      0x44
#define OP_MOV      0x51
#define OP_MOVLVAL  0x52
#define OP_CMP      0x53
#define OP_CMPLVAL  0x54
#define OP_HLT      0x60
#define OP_SYSCALL  0x61
#define OP_NOP      0x90

struct _instruction {
    uint8_t opcode;
    uint8_t operand_0;
    union {
        struct {
            uint8_t operand_1;
            uint8_t operand_2;
        } __attribute__((packed));
        uint16_t lval;
    };
} __attribute__((packed));


/*
*  Fills buffer with a string representation of the instruction
*  @return the size of string in buf, or -1 on error
*/
int ins_str (const struct _instruction *, char * buf, size_t buf_size);

const char * ins_reg_str    (uint8_t reg);
const char * ins_opcode_str (uint8_t reg);

#endif