#include "lexer.h"

#include <string.h>
#include <stdlib.h>

#include "instruction.h"

struct _lexer {
    struct _token * first;
    struct _token * last;
};

void lexer_append (struct _lexer * lexer, struct _token * token)
{
//    printf("new token: %d %s\n", token->type, token_str(token));
    if (lexer->last == NULL) {
        lexer->first = token;
        lexer->last  = token;
    }
    else {
        lexer->last->next = token;
        lexer->last = token;
    }
}

struct _token * lexer (const char * text)
{
    int i     = 0;
    int j, k  = 0;
    int line  = 1;
    char buf[64];

    struct _lexer lexer;
    lexer.first = NULL;
    lexer.last  = NULL;

    while (text[i] != '\0') {
        if (    (text[i] == ' ')
             || (text[i] == '\t')) {
            i++;
            continue;
        }

        if (text[i] == '\n') {
            lexer_append(&lexer, token_create(TOK_NEWLINE, NULL, line));
            i++;
            line++;
            continue;
        }

        if (text[i] == ',') {
            lexer_append(&lexer, token_create(TOK_COMMA, NULL, line));
            i++;
            continue;
        }

        if (text[i] == ';') {
            while (text[i] != '\n')
                i++;
            continue;
        }

        #define MATCHTOK(TOKSTR, TOKVAL) \
        if (strncmp(&(text[i]), #TOKSTR, strlen(#TOKSTR)) == 0) { \
            lexer_append(&lexer, token_create(TOKVAL, #TOKSTR, line)); \
            i += strlen(#TOKSTR); \
            continue; \
        }

        MATCHTOK(r0,      REG_0)
        MATCHTOK(r1,      REG_1)
        MATCHTOK(r2,      REG_2)
        MATCHTOK(r3,      REG_3)
        MATCHTOK(r4,      REG_4)
        MATCHTOK(r5,      REG_5)
        MATCHTOK(r6,      REG_6)
        MATCHTOK(r7,      REG_7)
        MATCHTOK(rsp,     REG_SP)
        MATCHTOK(rbp,     REG_BP)
        MATCHTOK(rip,     REG_IP)
        MATCHTOK(mov,     OP_MOV)
        MATCHTOK(cmp,     OP_CMP)
        MATCHTOK(add,     OP_ADD)
        MATCHTOK(sub,     OP_SUB)
        MATCHTOK(mul,     OP_MUL)
        MATCHTOK(div,     OP_DIV)
        MATCHTOK(mod,     OP_MOD)
        MATCHTOK(and,     OP_AND)
        MATCHTOK(or,      OP_OR)
        MATCHTOK(xor,     OP_XOR)
        MATCHTOK(jmp,     OP_JMP)
        MATCHTOK(je,      OP_JE)
        MATCHTOK(jne,     OP_JNE)
        MATCHTOK(jle,     OP_JLE)
        MATCHTOK(jl,      OP_JL)
        MATCHTOK(jge,     OP_JGE)
        MATCHTOK(jg,      OP_JG)
        MATCHTOK(call,    OP_CALL)
        MATCHTOK(ret,     OP_RET)
        MATCHTOK(loadb,   OP_LOADB)
        MATCHTOK(load,    OP_LOAD)
        MATCHTOK(storb,   OP_STORB)
        MATCHTOK(stor,    OP_STOR)
        MATCHTOK(in,      OP_IN)
        MATCHTOK(out,     OP_OUT)
        MATCHTOK(push,    OP_PUSH)
        MATCHTOK(pop,     OP_POP)
        MATCHTOK(hlt,     OP_HLT)
        MATCHTOK(syscall, OP_SYSCALL)
        MATCHTOK(nop,     OP_NOP)

        if (((text[i] >= '0') && (text[i] <= '9')) || (text[i] == '-')) {
            if ((text[i + 1] == 'x') && (text[i] != '-')) {
                for (j = i + 2;
                     (    ((text[j] >= '0') && (text[j] <= '9'))
                       || ((text[j] >= 'a') && (text[j] <= 'f')));
                     j++) {}
            }
            else {
                for (j = i + 1;
                     ((text[j] >= '0') && (text[j] <= '9'));
                     j++) {}
            }
            // catch #x case
            if (text[j - 1] != 'x') {
                strncpy(buf, &(text[i]), j - i < 64 ? j - i : 64);
                buf[j - i] = '\0'; // weird strncpy not copy null-terminator
                buf[63] = '\0';
                lexer_append(&lexer, token_create(TOK_NUMBER, buf, line));
                i = j;
                continue;
            }
        }

        if ((text[i] >= 'a') && (text[i] <= 'z')) {
            for (j = i + 1; 
                 ((text[j] >= 'a') && (text[j] <= 'z'))
                    || ((text[j] >= '0') && (text[j] <= '9'))
                    || (text[j] == '_');
                 j++) {}
            for (k = j; (text[k] == ' ') || (text[k] == '\t'); k++) {}
            strncpy(buf, &(text[i]), j - i < 64 ? j - i : 64);
            buf[j - i] = '\0';
            buf[63] = '\0';
            if (text[k] == ':') {
                lexer_append(&lexer, token_create(TOK_LABELDEC, buf, line));
                i = k + 1;
            }
            else {
                lexer_append(&lexer, token_create(TOK_LABEL, buf, line));
                i = j;
            }
            continue;
        }

        if (text[i] == '"') {
            char * tmpstr = (char *) malloc(4096);
            j = i + 1;
            k = 0;
            while (text[j] != '"') {
                if (text[j] == '\\') {
                    j++;
                    if (text[j] == 'n')
                        tmpstr[k++] = '\n';
                    else if (text[j] == 't')
                        tmpstr[k++] = '\t';
                    else
                        tmpstr[k++] = text[j];
                }
                else
                    tmpstr[k++] = text[j];
                j++;
            }
            tmpstr[k] = '\0';
            lexer_append(&lexer, token_create(TOK_STRING, tmpstr, line));
            i = j + 1;
            free(tmpstr);
            continue;
        }


        fprintf(stderr, "invalid input to lexer, line %d (char=0x%02x)\n",
                line, text[i]);

        return NULL;
    }

    if (lexer.last->type != TOK_NEWLINE)
        lexer_append(&lexer, token_create(TOK_NEWLINE, NULL, line));

    return lexer.first;
}