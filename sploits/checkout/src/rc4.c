#include "rc4.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define rotl(X, S) ((X << S) | (X >> (32 - S)))


int rc4_load_key (struct _rc4 * rc4, const char * filename)
{
    FILE * fh;
    size_t filesize;

    fh = fopen(filename, "rb");
    if (fh == NULL)
        return -1;

    fseek(fh, 0, SEEK_END);
    filesize = ftell(fh);
    fseek(fh, 0, SEEK_SET);

    if (filesize != 16)
        return -1;

    if (fread(rc4->key, 1, 16, fh) != filesize)
        return -1;

    fclose(fh);

    return 0;
}


void rc4_init (struct _rc4 * rc4)
{
    int i;
    int j;

    for (i = 0; i < 256; i++) {
        rc4->S[i] = i;
    }

    j = 0;
    for (i = 0; i < 256; i++) {
        j = (j + rc4->S[i] + rc4->key[i % 16]) % 256;
        if (i == j) continue;
        rc4->S[i] ^= rc4->S[j];
        rc4->S[j] ^= rc4->S[i];
        rc4->S[i] ^= rc4->S[j];
    }
}


uint8_t * rc4_encrypt (struct _rc4 * rc4, uint8_t * data, uint32_t data_len)
{
    uint32_t checksum;
    int data_i;
    int i, j;
    uint8_t k;

    uint8_t * result = (uint8_t *) malloc(data_len + 8);
    checksum = htonl(data_len);
    memcpy(result, &checksum, 4);
    memcpy(&(result[4]), data, data_len);

    i = 0;
    j = 0;
    checksum = 0;
    for (data_i = 0; data_i < data_len + 4; data_i++) {
        i = (i + 1) % 256;
        j = (j + rc4->S[i]) % 256;
        if (i != j) {
            rc4->S[i] ^= rc4->S[j];
            rc4->S[j] ^= rc4->S[i];
            rc4->S[i] ^= rc4->S[j];
        }
        k = rc4->S[(rc4->S[i] + rc4->S[j]) % 256];

        checksum += result[data_i];
        checksum = rotl(checksum, 8);

        result[data_i] ^= k;
    }

    checksum = htonl(checksum);
    memcpy(&(result[data_len + 4]), &checksum, 4);

    return result;
}


uint8_t * rc4_decrypt (struct _rc4 * rc4, uint8_t * data, uint32_t data_len)
{
    uint32_t checksum;
    uint32_t stored_checksum;
    uint8_t  stored_length[4];
    int data_i;
    int i, j;
    uint8_t k;

    if (data_len < 8)
        return NULL;

    uint8_t * result = (uint8_t *) malloc(data_len - 8);
    memcpy(result, &(data[4]), data_len - 8);
    memcpy(stored_length, data, 4);

    i = 0;
    j = 0;
    checksum = 0;
    for (data_i = 0; data_i < data_len - 4; data_i++) {
        i = (i + 1) % 256;
        j = (j + rc4->S[i]) % 256;
        if (i != j) {
            rc4->S[i] ^= rc4->S[j];
            rc4->S[j] ^= rc4->S[i];
            rc4->S[i] ^= rc4->S[j];
        }
        k = rc4->S[(rc4->S[i] + rc4->S[j]) % 256];

        if (data_i >= 4) {
            result[data_i - 4] ^= k;
            checksum += result[data_i - 4];
        }
        else {
            checksum += stored_length[data_i] ^ k;
        }
        checksum = rotl(checksum, 8);
    }

    memcpy(&stored_checksum, &(data[data_len - 4]), 4);
    stored_checksum = ntohl(stored_checksum);

    if (stored_checksum != checksum){
        free(result);
        return NULL;
    }
    return result;
}