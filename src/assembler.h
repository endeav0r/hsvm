#ifndef assembler_HEADER
#define assembler_HEADER

#include <inttypes.h>

#define ASSEM_REG  0x80
#define ASSEM_TERM 0x8f

// these must correspond with entries in rule_matrix
enum {
    RULE_MOV,
    RULE_MOVLVAL,
    RULE_MOVLABEL,
    RULE_CMP,
    RULE_CMPLVAL,
    RULE_ADD,
    RULE_ADDLVAL,
    RULE_SUB,
    RULE_SUBLVAL,
    RULE_MUL,
    RULE_MULLVAL,
    RULE_DIV,
    RULE_DIVLVAL,
    RULE_MOD,
    RULE_MODLVAL,
    RULE_AND,
    RULE_ANDLVAL,
    RULE_OR,
    RULE_ORLVAL,
    RULE_XOR,
    RULE_XORLVAL,
    RULE_JMP,
    RULE_JMPLVAL,
    RULE_JE,
    RULE_JELVAL,
    RULE_JNE,
    RULE_JNELVAL,
    RULE_JL,
    RULE_JLLVAL,
    RULE_JLE,
    RULE_JLELVAL,
    RULE_JG,
    RULE_JGLVAL,
    RULE_JGE,
    RULE_JGELVAL,
    RULE_CALL,
    RULE_CALLR,
    RULE_RET,
    RULE_LOAD,
    RULE_LOADR,
    RULE_LOADB,
    RULE_LOADBR,
    RULE_STOR,
    RULE_STORR,
    RULE_STORB,
    RULE_STORBR,
    RULE_IN,
    RULE_OUT,
    RULE_PUSH,
    RULE_PUSHLVAL,
    RULE_PUSHLABEL,
    RULE_POP,
    RULE_HLT,
    RULE_SYSCALL,
    RULE_NOP,
    RULE_LABELNUM,
    RULE_LABELSTR,
    RULE_LABEL,
    RULE_NEWLINE
};

struct _label {
    char * str;
    uint16_t location;
    struct _label * next;
};

struct _labels {
    struct _label * first;
    struct _label * last;
};

#endif