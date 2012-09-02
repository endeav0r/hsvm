#include "rc4.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char * argv[])
{
    struct _rc4 rc4;

    if (rc4_load_key(&rc4, "key")) {
        printf("failed loading key\n");
        return -1;
    }
    rc4_init(&rc4);

    unsigned char buf[512];
    uint32_t bytes_read;

    bytes_read = read(0, buf, 512);

    uint8_t * edata = rc4_decrypt(&rc4, buf, bytes_read);

    if (edata == NULL) {
        printf("checksum error\n");
    }
    else {
        printf("decryption successful\n");
        free(edata);
    }


    return 0;
}