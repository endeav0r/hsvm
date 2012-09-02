#ifndef rc4_HEADER
#define rc4_HEADER

#include <inttypes.h>

struct _rc4 {
    uint8_t S[256];
    uint8_t key[16];
};

int       rc4_load_key (struct _rc4 * rc4, const char * filename);
void      rc4_init     (struct _rc4 * rc4);
uint8_t * rc4_encrypt  (struct _rc4 * rc4, uint8_t * data, uint32_t data_len);
uint8_t * rc4_decrypt  (struct _rc4 * rc4, uint8_t * data, uint32_t data_len);

#endif