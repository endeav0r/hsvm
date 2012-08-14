#include "token.h"

#include <stdlib.h>
#include <string.h>

#include "instruction.h"

struct _token * token_create (int type, const char * str, int line)
{
    struct _token * token = (struct _token *) malloc(sizeof(struct _token));
    token->type = type;
    token->next = NULL;
    token->line = line;

    if (str == NULL)
        token->str = NULL;
    else {
        token->str = (char *) malloc(strlen(str) + 1);
        strcpy(token->str, str);
        token->str[strlen(str)] = '\0';
    }

    return token;
}


void token_destroy (struct _token * token)
{
    if (token->str != NULL)
        free(token->str);
    free(token);
}


const char * token_str (struct _token * token)
{
    if (ins_reg_str(token->type) != NULL)
        return ins_reg_str(token->type);
    if (ins_opcode_str(token->type) != NULL)
        return ins_opcode_str(token->type);
    if (token->type == TOK_NEWLINE)
        return "NEWLINE";
    if (token->type == TOK_COMMA)
        return ",";
    return token->str;
}