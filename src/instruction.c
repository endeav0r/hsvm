#include "instruction.h"

#ifdef WINDOWS
    #include <winsock2.h>
#else
    #include <arpa/inet.h>
#endif

#include <stdio.h>

const char * ins_reg_str (uint8_t reg) {
    switch (reg) {
    case REG_0  : return "r0";
    case REG_1  : return "r1";
    case REG_2  : return "r2";
    case REG_3  : return "r3";
    case REG_4  : return "r4";
    case REG_5  : return "r5";
    case REG_6  : return "r6";
    case REG_7  : return "r7";
    case REG_SP : return "rsp";
    case REG_BP : return "rbp";
    case REG_IP : return "rip";
    }
    return NULL;
}

const char * ins_opcode_str (uint8_t opcode) {
    #define OPCODECASE(XX, XXSTR) case OP_##XX : return #XXSTR;
    switch (opcode) {
    OPCODECASE(ADD,      add)
    OPCODECASE(ADDLVAL,  add)
    OPCODECASE(SUB,      sub)
    OPCODECASE(SUBLVAL,  sub)
    OPCODECASE(MUL,      mul)
    OPCODECASE(MULLVAL,  mul)
    OPCODECASE(DIV,      div)
    OPCODECASE(DIVLVAL,  div)
    OPCODECASE(MOD,      mod)
    OPCODECASE(MODLVAL,  mod)
    OPCODECASE(AND,      and)
    OPCODECASE(ANDLVAL,  and)
    OPCODECASE(OR,       or)
    OPCODECASE(ORLVAL,   or)
    OPCODECASE(XOR,      xor)
    OPCODECASE(XORLVAL,  xor)
    OPCODECASE(JMP,      jmp)
    OPCODECASE(JE,       je)
    OPCODECASE(JNE,      jne)
    OPCODECASE(JL,       jl)
    OPCODECASE(JLE,      jle)
    OPCODECASE(JG,       jg)
    OPCODECASE(JGE,      jge)
    OPCODECASE(CALL,     call)
    OPCODECASE(CALLR,    call)
    OPCODECASE(RET,      ret)
    OPCODECASE(LOAD,     load)
    OPCODECASE(LOADR,    load)
    OPCODECASE(LOADB,    loadb)
    OPCODECASE(LOADBR,   loadb)
    OPCODECASE(STOR,     stor)
    OPCODECASE(STORR,    stor)
    OPCODECASE(STORB,    storb)
    OPCODECASE(STORBR,   storb)
    OPCODECASE(IN,       in)
    OPCODECASE(OUT,      out)
    OPCODECASE(PUSH,     push)
    OPCODECASE(PUSHLVAL, push)
    OPCODECASE(POP,      pop)
    OPCODECASE(MOV,      mov)
    OPCODECASE(MOVLVAL,  mov)
    OPCODECASE(CMP,      cmp)
    OPCODECASE(CMPLVAL,  cmp)
    OPCODECASE(HLT,      hlt)
    OPCODECASE(SYSCALL,  syscall)
    OPCODECASE(NOP,      nop)
    }
    return NULL;
}

int ins_str (const struct _instruction * ins, char * buf, size_t buf_size)
{
    const char * opcode    = ins_opcode_str(ins->opcode);
    const char * operand_0 = ins_reg_str(ins->operand_0);
    const char * operand_1 = ins_reg_str(ins->operand_1);
    const char * operand_2 = ins_reg_str(ins->operand_2);
    char lval[8];

    if (opcode == NULL)
        return -1;

    switch (ins->opcode) {
    case OP_ADD :
    case OP_SUB :
    case OP_MUL :
    case OP_DIV :
    case OP_MOD :
    case OP_AND :
    case OP_OR  :
    case OP_XOR :
        if ((operand_0 == NULL) || (operand_1 == NULL) || (operand_2 == NULL))
            return -1;
        return snprintf(buf, buf_size, "%s %s, %s, %s",
                        opcode, operand_0, operand_1, operand_2);

    case OP_ADDLVAL :
    case OP_SUBLVAL :
    case OP_MULLVAL :
    case OP_DIVLVAL :
    case OP_MODLVAL :
    case OP_ANDLVAL :
    case OP_ORLVAL  :
    case OP_XORLVAL :
        if (operand_0 == NULL)
            return -1;
        snprintf(lval, 8, "%04x", ntohs(ins->lval));
        return snprintf(buf, buf_size, "%s %s, %s", opcode, operand_0, lval);

    case OP_MOV    :
    case OP_CMP    :
    case OP_LOADR  :
    case OP_STORR  :
    case OP_LOADBR :
    case OP_STORBR :
        if ((operand_0 == NULL) || (operand_1 == NULL))
            return -1;
        return snprintf(buf, buf_size, "%s %s, %s", opcode, operand_0, operand_1);

    case OP_JMP      :
    case OP_JE       :
    case OP_JNE      :
    case OP_JL       :
    case OP_JLE      :
    case OP_JG       :
    case OP_JGE      :
    case OP_CALL     :
    case OP_PUSHLVAL :
        snprintf(lval, 8, "%d", ntohs(ins->lval));
        return snprintf(buf, buf_size, "%s %s", opcode, lval);

    case OP_LOAD    :
    case OP_STOR    :
    case OP_LOADB   :
    case OP_STORB   :
    case OP_MOVLVAL :
    case OP_CMPLVAL :
        if (operand_0 == NULL)
            return -1;
        snprintf(lval, 8, "%04x", ntohs(ins->lval));
        if (ins->opcode == OP_STOR) 
            return snprintf(buf, buf_size, "%s %s, %s", opcode, lval, operand_0);
        else
            return snprintf(buf, buf_size, "%s %s, %s", opcode, operand_0, lval);

    case OP_CALLR :
    case OP_IN    :
    case OP_OUT   :
    case OP_PUSH  :
    case OP_POP   :
        if (operand_0 == NULL)
            return -1;
        return snprintf(buf, buf_size, "%s %s", opcode, operand_0);

    case OP_RET     :
    case OP_HLT     :
    case OP_SYSCALL :
    case OP_NOP     :
        return snprintf(buf, buf_size, "%s", opcode);
    }

    return -1;
}