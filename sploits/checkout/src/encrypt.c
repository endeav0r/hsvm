#include "rc4.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[])
{
    struct _rc4 rc4;

    if (rc4_load_key(&rc4, "key")) {
        printf("failed loading key\n");
        return -1;
    }
    rc4_init(&rc4);

    unsigned char buf[1024];
    uint32_t bytes_read;
    bytes_read = fread(buf, 1, 1024, stdin);

    uint8_t * edata = rc4_encrypt(&rc4, buf, bytes_read);

    fwrite(edata, 1, bytes_read + 8, stdout);

    free(edata);

    return 0;
}