#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define ROTL16(x,s) ((x << s) | (x >> (16 - s)))

uint64_t sum (uint16_t * M)
{
    uint16_t a, b, c, d;
    a = 0xdead;
    b = 0xbeef;
    c = 0xf0f0;
    d = 0x0f0f;

    a = (ROTL16(d, 3) + ((a | b) ^ c)) ^ M[0];
    b = (ROTL16(a, 7) + ((b | c) ^ d)) ^ M[1];
    c = (ROTL16(b, 3) + ((c | d) ^ a)) ^ M[2];
    d = (ROTL16(c, 7) + ((d | a) ^ b)) ^ M[3];

    a = (ROTL16(d, 3) + ((a & b) ^ c)) ^ M[3];
    b = (ROTL16(a, 3) + ((b & c) ^ d)) ^ M[2];
    c = (ROTL16(b, 7) + ((c & d) ^ a)) ^ M[1];
    d = (ROTL16(c, 7) + ((d & a) ^ b)) ^ M[0];

    a = (ROTL16(d, 7) + (a ^ b ^ c)) ^ M[1];
    b = (ROTL16(a, 7) + (b ^ c ^ d)) ^ M[3];
    c = (ROTL16(b, 3) + (c ^ d ^ a)) ^ M[0];
    d = (ROTL16(c, 3) + (d ^ a ^ b)) ^ M[2];

    uint64_t result = 0;
    result |= a;
    result <<= 16;
    result |= b;
    result <<= 16;
    result |= c;
    result <<= 16;
    result |= d;

    return result;
}

int main ()
{
    int i;
    uint16_t input[4];

    for (i = 0; i < 4; i++) {
        scanf("%04hx", &(input[i]));
    }

    uint64_t result = sum(input);

    if (result == 0) {
        FILE * fh;
        size_t filesize;
        char * buf;

        fh = fopen("flag", "r");

        fseek(fh, 0, SEEK_END);
        filesize = ftell(fh);
        fseek(fh, 0, SEEK_SET);

        buf = malloc(filesize + 1);
        memset(buf, 0, filesize + 1);

        fread(buf, 1, filesize, fh);

        fclose(fh);

        printf("%s\n", buf);
        free(buf);
    }
    else {
        for (i = 0; i < 4; i++) {
            printf("%04hx ", input[i]);
        }
        printf(" is invalid\n");
    }

    return 0;
}