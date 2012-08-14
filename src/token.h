#ifndef token_HEADER
#define token_HEADER

#define TOK_NEWLINE  0x70
#define TOK_COMMA    0x71
#define TOK_NUMBER   0x72
#define TOK_LABELDEC 0x73
#define TOK_LABEL    0x74
#define TOK_STRING   0x75

struct _token {
    int type;
    int line;
    char * str;
    struct _token * next;
};

struct _token * token_create  (int type, const char * str, int line);
void            token_destroy (struct _token *);

const char * token_str (struct _token *);

#endif