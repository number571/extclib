#include <stdio.h>
#include <string.h>

#include "extclib/crypto.h"

/*
[ 31 32 33 34 35 36 37 38 39 30 61 62 63 64 65 66 67 68 69 6a 0 0 0 0 0 0 0 0 0 0 0 0 ]
[ 95 3c f3 98 3c f8 95 68 93 16 be 9d 6d 64 fa 73 3c 20 22 15 66 83 f0 b4 74 d2 80 27 c0 e0 53 f3 ]
[ 31 32 33 34 35 36 37 38 39 30 61 62 63 64 65 66 67 68 69 6a 0 0 0 0 0 0 0 0 0 0 0 0 ]
*/

#define MAX_SIZE 32 // multiple 128 bit for AES block

void print_bytes(uint8_t * array, size_t length);

int main(void) {
    uint8_t in[BUFSIZ] = "1234567890abcdefghij";

    Context params = {
        .mode = CBC_MODE,
        .data = {
            .size = strlen((char*)in),
            .in = in,
            .out = in,
        },
        .desc = {
            .size = 128,
            .vec1 = (uint8_t*)"AES_key_128_bits", // key
            .vec2 = (uint8_t*)"abcdefghijklmnop", // iv
        },
    };

    print_bytes(in, MAX_SIZE);

    aes_crypto(ENCRYPT_OPTION, params);
    print_bytes(in, MAX_SIZE);

    aes_crypto(DECRYPT_OPTION, params);
    print_bytes(in, MAX_SIZE);

    return 0;
}

void print_bytes(uint8_t * array, size_t length) {
    printf("[ ");
    for (size_t i = 0; i < length; ++i)
        printf("%x ", array[i]);
    printf("]\n");
}
