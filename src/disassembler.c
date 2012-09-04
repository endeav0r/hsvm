#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "instruction.h"

unsigned char * disassembler_load (const char * filename, size_t * filesize)
{
    FILE * fh;
    unsigned char * buf;

    fh = fopen(filename, "rb");
    if (fh == NULL) {
        fprintf(stderr, "could not open file %s\n", filename);
        return NULL;
    }

    fseek(fh, 0, SEEK_END);
    *filesize = ftell(fh);
    fseek(fh, 0, SEEK_SET);

    buf = (unsigned char *) malloc(*filesize);

    fread(buf, 1, *filesize, fh);

    fclose(fh);

    return buf;
}


int is_branch (uint8_t * buf)
{
    struct _instruction * ins = (struct _instruction *) buf;
    switch (ins->opcode) {
        case OP_JMP  :
        case OP_JE   :
        case OP_JNE  :
        case OP_JL   :
        case OP_JLE  :
        case OP_JG   :
        case OP_JGE  :
        case OP_CALL :
            return 1;
    };
    return 0;
}


// oh god this hurts
uint16_t branch_abs (uint8_t * buf, size_t ip)
{
    struct _instruction * ins = (struct _instruction *) buf;

    return ip + ntohs(ins->lval);
}


int main (int argc, char * argv[])
{
    int c;
    unsigned int offset = 0;

    while ((c = getopt(argc, argv, "o:")) != -1) {
        switch (c) {
        case 'o' :
            offset = strtoul(optarg, NULL, 0);
            break;
        }
    }

    if (optind == argc) {
        fprintf(stderr, "missing name of file to disassembler\n");
        return -1;
    }

    uint8_t * buf;
    size_t buf_size;

    buf = disassembler_load(argv[optind], &buf_size);
    if (buf == NULL)
        return -1;

    size_t buf_i = offset;
    char dis_str[64];
    int error;

    while (buf_i < buf_size) {
        if (buf_i <= buf_size - 4) {
            error = ins_str((struct _instruction *) &(buf[buf_i]), dis_str, 64);
            if (error > 0) {
                // destination for jump instructions
                if (is_branch(&(buf[buf_i]))) {
                    printf("%04x %s (%04x)\n", (unsigned int) buf_i, dis_str,
                           branch_abs(&(buf[buf_i]), buf_i + 4));
                }
                else {
                    printf("%04x %s\n", (unsigned int) buf_i, dis_str);
                }
                buf_i += 4;
                continue;
            }
        }
        if (buf_i <= buf_size - 2) {
            printf("%04x %02x%02x\n", (unsigned int) buf_i, buf[buf_i], buf[buf_i + 1]);
            buf_i += 2;
        }
        else
            buf_i++;
    }

    free(buf);

    return 0;
}