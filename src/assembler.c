#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "assembler.h"
#include "instruction.h"
#include "lexer.h"
#include "token.h"


uint8_t rule_matrix [][8] = {
    {OP_MOV,   ASSEM_REG, TOK_COMMA, ASSEM_REG,  TOK_NEWLINE, ASSEM_TERM},
    {OP_MOV,   ASSEM_REG, TOK_COMMA, TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_MOV,   ASSEM_REG, TOK_COMMA, TOK_LABEL,  TOK_NEWLINE, ASSEM_TERM},
    {OP_CMP,   ASSEM_REG, TOK_COMMA, ASSEM_REG,  TOK_NEWLINE, ASSEM_TERM},
    {OP_CMP,   ASSEM_REG, TOK_COMMA, TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_ADD,   ASSEM_REG, TOK_COMMA, ASSEM_REG, TOK_COMMA, ASSEM_REG,  TOK_NEWLINE, ASSEM_TERM},
    {OP_ADD,   ASSEM_REG, TOK_COMMA, TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_SUB,   ASSEM_REG, TOK_COMMA, ASSEM_REG, TOK_COMMA, ASSEM_REG,  TOK_NEWLINE, ASSEM_TERM},
    {OP_SUB,   ASSEM_REG, TOK_COMMA, TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_MUL,   ASSEM_REG, TOK_COMMA, ASSEM_REG, TOK_COMMA, ASSEM_REG,  TOK_NEWLINE, ASSEM_TERM},
    {OP_MUL,   ASSEM_REG, TOK_COMMA, TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_DIV,   ASSEM_REG, TOK_COMMA, ASSEM_REG, TOK_COMMA, ASSEM_REG,  TOK_NEWLINE, ASSEM_TERM},
    {OP_DIV,   ASSEM_REG, TOK_COMMA, TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_MOD,   ASSEM_REG, TOK_COMMA, ASSEM_REG, TOK_COMMA, ASSEM_REG,  TOK_NEWLINE, ASSEM_TERM},
    {OP_MOD,   ASSEM_REG, TOK_COMMA, TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_AND,   ASSEM_REG, TOK_COMMA, ASSEM_REG, TOK_COMMA, ASSEM_REG,  TOK_NEWLINE, ASSEM_TERM},
    {OP_AND,   ASSEM_REG, TOK_COMMA, TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_OR,    ASSEM_REG, TOK_COMMA, ASSEM_REG, TOK_COMMA, ASSEM_REG,  TOK_NEWLINE, ASSEM_TERM},
    {OP_OR,    ASSEM_REG, TOK_COMMA, TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_XOR,   ASSEM_REG, TOK_COMMA, ASSEM_REG, TOK_COMMA, ASSEM_REG,  TOK_NEWLINE, ASSEM_TERM},
    {OP_XOR,   ASSEM_REG, TOK_COMMA, TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_JMP,   TOK_LABEL,  TOK_NEWLINE, ASSEM_TERM},
    {OP_JMP,   TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_JE,    TOK_LABEL,  TOK_NEWLINE, ASSEM_TERM},
    {OP_JE,    TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_JNE,   TOK_LABEL,  TOK_NEWLINE, ASSEM_TERM},
    {OP_JNE,   TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_JL,    TOK_LABEL,  TOK_NEWLINE, ASSEM_TERM},
    {OP_JL,    TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_JLE,   TOK_LABEL,  TOK_NEWLINE, ASSEM_TERM},
    {OP_JLE,   TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_JG,    TOK_LABEL,  TOK_NEWLINE, ASSEM_TERM},
    {OP_JG,    TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_JGE,   TOK_LABEL,  TOK_NEWLINE, ASSEM_TERM},
    {OP_JGE,   TOK_NUMBER, TOK_NEWLINE, ASSEM_TERM},
    {OP_CALL,  TOK_LABEL, TOK_NEWLINE, ASSEM_TERM},
    {OP_CALL,  ASSEM_REG, TOK_NEWLINE, ASSEM_TERM},
    {OP_RET,   TOK_NEWLINE, ASSEM_TERM},
    {OP_LOAD,  ASSEM_REG, TOK_COMMA, TOK_LABEL, TOK_NEWLINE, ASSEM_TERM},
    {OP_LOAD,  ASSEM_REG, TOK_COMMA, ASSEM_REG, TOK_NEWLINE, ASSEM_TERM},
    {OP_LOADB, ASSEM_REG, TOK_COMMA, TOK_LABEL, TOK_NEWLINE, ASSEM_TERM},
    {OP_LOADB, ASSEM_REG, TOK_COMMA, ASSEM_REG, TOK_NEWLINE, ASSEM_TERM},
    {OP_STOR,  TOK_LABEL, TOK_COMMA, ASSEM_REG, TOK_NEWLINE, ASSEM_TERM},
    {OP_STOR,  ASSEM_REG, TOK_COMMA, ASSEM_REG, TOK_NEWLINE, ASSEM_TERM},
    {OP_STORB, TOK_LABEL, TOK_COMMA, ASSEM_REG, TOK_NEWLINE, ASSEM_TERM},
    {OP_STORB, ASSEM_REG, TOK_COMMA, ASSEM_REG, TOK_NEWLINE, ASSEM_TERM},
    {OP_IN,    ASSEM_REG, TOK_NEWLINE, ASSEM_TERM},
    {OP_OUT,   ASSEM_REG, TOK_NEWLINE, ASSEM_TERM},
    {OP_PUSH,  ASSEM_REG, ASSEM_TERM},
    {OP_PUSH,  TOK_NUMBER, ASSEM_TERM},
    {OP_PUSH,  TOK_LABEL, ASSEM_TERM},
    {OP_POP,   ASSEM_REG, ASSEM_TERM},
    {OP_HLT,   TOK_NEWLINE, ASSEM_TERM},
    {OP_SYSCALL, TOK_NEWLINE, ASSEM_TERM},
    {OP_NOP,   TOK_NEWLINE, ASSEM_TERM},
    {TOK_LABELDEC, TOK_NUMBER, ASSEM_TERM},
    {TOK_LABELDEC, TOK_STRING, ASSEM_TERM},
    {TOK_LABELDEC, TOK_LABEL, ASSEM_TERM},
    {TOK_LABELDEC, ASSEM_TERM},
    {TOK_NEWLINE, ASSEM_TERM},
    {ASSEM_TERM}
};


uint8_t token_to_assem (struct _token * token)
{
    switch (token->type) {
    case REG_0  :
    case REG_1  :
    case REG_2  :
    case REG_3  :
    case REG_4  :
    case REG_5  :
    case REG_6  :
    case REG_7  :
    case REG_SP :
    case REG_BP :
    case REG_IP :
        return ASSEM_REG;
    }
    return token->type;
}


int match_rule (struct _token * tokens)
{
    struct _token * next;
    int i, j;

    for (i = 0; rule_matrix[i][0] != ASSEM_TERM; i++) {
        j = 0;
        next = tokens;
        while (1) {
            if (rule_matrix[i][j] == ASSEM_TERM)
                return i;
            if (next == NULL)
                break;
            if (rule_matrix[i][j] != token_to_assem(next))
                break;
            next = next->next;
            j++;
        }
    }
    return -1;
}


int rule_size (int rule)
{
    int size;
    for (size = 0; rule_matrix[rule][size] != ASSEM_TERM; size++) {}
    return size;
}


struct _token * advance_token (struct _token * token, int steps)
{
    int i;
    for (i = 0; i < steps; i++) {
        token = token->next;
    }
    return token;
}


struct _token * token_peek (struct _token * token, int depth)
{
    int i;
    for (i = 0; i < depth; i++) {
        token = token->next;
    }
    return token;
}


struct _label * label_create (const char * str, uint16_t location)
{
    struct _label * label;

    label = (struct _label *) malloc(sizeof(struct _label));
    label->str = (char *) malloc(strlen(str) + 1);
    strcpy(label->str, str);
    label->location = location;
    label->next = NULL;

    return label;
}


void labels_append (struct _labels * labels, struct _label * label)
{
    if (labels->first == NULL) {
        labels->first = label;
        labels->last  = label;
    }
    else {
        labels->last->next = label;
        labels->last = label;
    }
}


struct _label * labels_search (struct _labels * labels, const char * needle)
{
    struct _label * next;
    for (next = labels->first; next != NULL; next = next->next) {
        if (strcmp(next->str, needle) == 0)
            return next;
    }
    return NULL;
}


uint16_t assemble (struct _token * tokens, unsigned char * mem)
{
    struct _labels labels;
    struct _label * label;
    struct _instruction ins;
    struct _token * next;
    struct _token * peek;
    int rule;
    int mem_i = 0;
    int copy_instruction = 0;
    uint16_t lval;

    labels.first = NULL;
    labels.last = NULL;

    // first pass to assemble
    next = tokens;
    while (next != NULL) {
        memset(&ins, 0, sizeof(struct _instruction));
        rule = match_rule(next);
        copy_instruction = 0;

        if (rule == -1) {
            fprintf(stderr, "couldn't match rule, line %d\n", next->line);
            for (peek = next;
                 (peek != NULL) && (peek->type != TOK_NEWLINE);
                 peek = peek->next) {
                fprintf(stderr, "%s ", token_str(peek));
            }
            fprintf(stderr, "\n");
            exit(-1);
        }

        switch (rule) {
        case RULE_MOV :
        case RULE_CMP :
            ins.opcode = next->type;
            peek = token_peek(next, 1);
            ins.operand_0 = peek->type;
            peek = token_peek(next, 3);
            ins.operand_1 = peek->type;
            copy_instruction = 1;
            break;

        case RULE_MOVLVAL    :
        case RULE_MOVLABEL   :
        case RULE_CMPLVAL    :
        case RULE_ADDLVAL    :
        case RULE_SUBLVAL    :
        case RULE_MULLVAL    :
        case RULE_DIVLVAL    :
        case RULE_MODLVAL    :
        case RULE_ANDLVAL    :
        case RULE_ORLVAL     :
        case RULE_XORLVAL    :
            if (next->type == OP_MOV) ins.opcode = OP_MOVLVAL;
            else if (next->type == OP_CMP) ins.opcode = OP_CMPLVAL;
            else if (next->type == OP_ADD) ins.opcode = OP_ADDLVAL;
            else if (next->type == OP_SUB) ins.opcode = OP_SUBLVAL;
            else if (next->type == OP_MUL) ins.opcode = OP_MULLVAL;
            else if (next->type == OP_DIV) ins.opcode = OP_DIVLVAL;
            else if (next->type == OP_MOD) ins.opcode = OP_MODLVAL;
            else if (next->type == OP_AND) ins.opcode = OP_ANDLVAL;
            else if (next->type == OP_OR ) ins.opcode = OP_ORLVAL;
            else if (next->type == OP_XOR) ins.opcode = OP_XORLVAL;
            peek = token_peek(next, 1);
            ins.operand_0 = peek->type;
            peek = token_peek(next, 3);
            lval = strtoul(peek->str, NULL, 0);
            lval = htons(lval);
            ins.lval = lval;
            copy_instruction = 1;
            break;

        case RULE_LOAD  :
        case RULE_STOR  :
        case RULE_LOADB :
        case RULE_STORB :
        case RULE_IN    :
        case RULE_OUT   :
        case RULE_PUSH  :
        case RULE_POP   :
            ins.opcode = next->type;
            if ((rule == RULE_STOR) || (rule == RULE_STORB))
                peek = token_peek(next, 3);
            else
                peek = token_peek(next, 1);
            ins.operand_0 = peek->type;
            copy_instruction = 1;
            break;

        case RULE_ADD :
        case RULE_SUB :
        case RULE_MUL :
        case RULE_DIV :
        case RULE_MOD :
        case RULE_AND :
        case RULE_OR  :
        case RULE_XOR :
            ins.opcode = next->type;
            peek = token_peek(next, 1);
            ins.operand_0 = peek->type;
            peek = token_peek(next, 3);
            ins.operand_1 = peek->type;
            peek = token_peek(next, 5);
            ins.operand_2 = peek->type;
            copy_instruction = 1;
            break;

        case RULE_JMP     :
        case RULE_JE      :
        case RULE_JNE     :
        case RULE_JL      :
        case RULE_JLE     :
        case RULE_JG      :
        case RULE_JGE     :
        case RULE_CALL    :
        case RULE_RET     :
        case RULE_HLT     :
        case RULE_SYSCALL :
        case RULE_NOP     :
        case RULE_PUSHLABEL :
            if (rule == RULE_PUSHLABEL)
                ins.opcode = OP_PUSHLVAL;
            else
                ins.opcode = next->type;
            copy_instruction = 1;
            break;

        case RULE_LOADR  :
        case RULE_STORR  :
        case RULE_LOADBR :
        case RULE_STORBR :
            if (rule == RULE_LOADR)
                ins.opcode = OP_LOADR;
            else if (rule == RULE_STORR)
                ins.opcode = OP_STORR;
            else if (rule == RULE_LOADBR)
                ins.opcode = OP_LOADBR;
            else if (rule == RULE_STORBR)
                ins.opcode = OP_STORBR;
            peek = token_peek(next, 1);
            ins.operand_0 = peek->type;
            peek = token_peek(next, 3);
            ins.operand_1 = peek->type;
            copy_instruction = 1;
            break;

        case RULE_PUSHLVAL :
        case RULE_JMPLVAL  :
        case RULE_JELVAL   :
        case RULE_JNELVAL  :
        case RULE_JLLVAL   :
        case RULE_JLELVAL  :
        case RULE_JGLVAL   :
        case RULE_JGELVAL  :
            if (rule == RULE_PUSHLVAL) ins.opcode = OP_PUSHLVAL;
            else ins.opcode = next->type;
            peek = token_peek(next, 1);
            lval = strtoul(peek->str, NULL, 0);
            lval = htons(lval);
            ins.lval = lval;
            copy_instruction = 1;
            break;

        case RULE_LABELNUM :
            labels_append(&labels, label_create(next->str, mem_i));

            peek = token_peek(next, 1);
            lval = strtoul(peek->str, NULL, 0);
            lval = htons(lval);
            memcpy(&(mem[mem_i]), &lval, 2);
            mem_i += 2;
            break;

        case RULE_LABELSTR :
            labels_append(&labels, label_create(next->str, mem_i));

            peek = token_peek(next, 1);
            memcpy(&(mem[mem_i]), peek->str, strlen(peek->str) + 1);
            mem_i += strlen(peek->str) + 1;
            break;

        case RULE_LABELLABEL :
            labels_append(&labels, label_create(next->str, mem_i));
            mem_i += 2;
            break;

        case RULE_LABEL :
            labels_append(&labels, label_create(next->str, mem_i));
            break;
        }

        if (copy_instruction) {
            memcpy(&(mem[mem_i]), &ins, sizeof(struct _instruction));
            mem_i += sizeof(struct _instruction);
        }
        next = advance_token(next, rule_size(rule));
    }

    // second pass to patch labels
    mem_i = 0;
    next = tokens;
    while (next != NULL) {
        rule = match_rule(next);
        switch (rule) {
        case RULE_JMP       :
        case RULE_JE        :
        case RULE_JNE       :
        case RULE_JL        :
        case RULE_JLE       :
        case RULE_JG        :
        case RULE_JGE       :
        case RULE_CALL      :
        case RULE_LOAD      :
        case RULE_LOADB     :
        case RULE_STOR      :
        case RULE_STORB     :
        case RULE_MOVLABEL  :
        case RULE_PUSHLABEL :

            if ((rule == RULE_LOAD) || (rule == RULE_MOVLABEL))
                peek = token_peek(next, 3);
            else
                peek = token_peek(next, 1);

            label = labels_search(&labels, peek->str);
            if (label == NULL) {
                fprintf(stderr, "could not find label: %s\n", peek->str);
                exit(-1);
            }

            memcpy(&ins, &(mem[mem_i]), sizeof(struct _instruction));

            ins.lval = label->location;
            if (    (rule == RULE_JMP)   || (rule == RULE_JE)  || (rule == RULE_JNE)
                 || (rule == RULE_JL)    || (rule == RULE_JLE)
                 || (rule == RULE_JG)    || (rule == RULE_JGE)
                 || (rule == RULE_CALL)) {
                ins.lval = ins.lval - mem_i - 4;
            }
            ins.lval = htons(ins.lval);

            memcpy(&(mem[mem_i]), &ins, sizeof(struct _instruction));

        case RULE_MOV       :
        case RULE_MOVLVAL   :
        case RULE_CMP       :
        case RULE_CMPLVAL   :
        case RULE_ADD       :
        case RULE_ADDLVAL   :
        case RULE_SUB       :
        case RULE_SUBLVAL   :
        case RULE_MUL       :
        case RULE_MULLVAL   :
        case RULE_DIV       :
        case RULE_DIVLVAL   :
        case RULE_MOD       :
        case RULE_MODLVAL   :
        case RULE_AND       :
        case RULE_ANDLVAL   :
        case RULE_OR        :
        case RULE_ORLVAL    :
        case RULE_XOR       :
        case RULE_XORLVAL   :
        case RULE_JMPLVAL   :
        case RULE_JELVAL    :
        case RULE_JNELVAL   :
        case RULE_JLLVAL    :
        case RULE_JLELVAL   :
        case RULE_JGLVAL    :
        case RULE_JGELVAL   :
        case RULE_CALLR     :
        case RULE_RET       :
        case RULE_LOADR     :
        case RULE_LOADBR    :
        case RULE_STORR     :
        case RULE_STORBR    :
        case RULE_IN        :
        case RULE_OUT       :
        case RULE_PUSH      :
        case RULE_PUSHLVAL  :
        case RULE_POP       :
        case RULE_HLT       :
        case RULE_SYSCALL   :
        case RULE_NOP       :
            mem_i += sizeof(struct _instruction);
            break;

        case RULE_LABELLABEL :
            peek = token_peek(next, 1);

            label = labels_search(&labels, peek->str);
            if (label == NULL) {
                fprintf(stderr, "could not find label: %s\n", peek->str);
                exit(-1);
            }
            ins.lval = htons(label->location);
            memcpy(&(mem[mem_i]), &(ins.lval), 2);

        case RULE_LABELNUM :
            mem_i += 2;
            break;

        case RULE_LABELSTR :
            peek = token_peek(next, 1);
            mem_i += strlen(peek->str) + 1;
            break;
        }
        next = advance_token(next, rule_size(rule));
    }

    return mem_i;
}


struct _token * get_tokens (const char * filename)
{
    FILE * fh;
    size_t filesize;
    char * buf;

    fh = fopen(filename, "r");
    if (fh == NULL) {
        fprintf(stderr, "could not open file %s\n", filename);
        return NULL;
    }

    fseek(fh, 0, SEEK_END);
    filesize = ftell(fh);
    fseek(fh, 0, SEEK_SET);

    buf = (char *) malloc(filesize + 1);

    fread(buf, 1, filesize, fh);
    buf[filesize] = '\0';

    fclose(fh);

    struct _token * tokens = lexer(buf);

    if (tokens == NULL)
        printf("lexer error in file: %s\n", filename);

    free(buf);

    return tokens;
}


int write_mem (const char * filename, unsigned char * mem, int mem_size)
{
    FILE * fh;

    fh = fopen(filename, "wb");
    if (fh == NULL) {
        fprintf(stderr, "could not open file %s\n", filename);
        return -1;
    }

    fwrite(mem, 1, mem_size, fh);

    fclose(fh);

    return 0;
}


int main (int argc, char * argv[])
{
    int c;
    char * output_filename = NULL;

    while ((c = getopt(argc, argv, "o:")) != -1) {
        switch (c) {
        case 'o' :
            output_filename = optarg;
            break;
        case '?' :
            if (optopt == 'o')
                printf("argument o requires argument\n");
            else
                printf("unknown argument %c\n", c);
            return -1;
        }
    }

    if (output_filename == NULL) {
        printf("usage: %s -o filename <sources>\n", argv[0]);
        printf("\n");
        printf(" -o specifies output filename\n");
        printf(" <sources> specifices assembly files for input\n");
        return -1;
    }

    struct _token * first = NULL;
    struct _token * last = NULL;
    int source_i;

    for (source_i = optind; source_i < argc; source_i++) {
        if (first == NULL) {
            first = get_tokens(argv[source_i]);
            if (first == NULL)
                return -1;
            last = first;
        }
        else {
            last->next = get_tokens(argv[source_i]);
            if (last->next == NULL)
                return -1;
        }
        while (last->next != NULL) { last = last->next; }
    }

    unsigned char * mem;
    mem = (unsigned char *) malloc(0x10000);
    int assembled_bytes = assemble(first, mem);

    write_mem(output_filename, mem, assembled_bytes);

    free(mem);

    return 0;
}