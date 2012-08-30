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

    unsigned char * buf;
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
                printf("%04x %s\n", (unsigned int) buf_i, dis_str);
                buf_i += 4;
                continue;
            }
        }
        if (buf_i <= buf_size - 2) {
            printf("%04x %02x%02x\n", (unsigned int) buf_i, buf[buf_i], buf[buf_i + 1]);
            buf_i += 2;
        }
        buf_i++;
    }

    free(buf);

    return 0;
}