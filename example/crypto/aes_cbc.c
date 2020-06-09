#include <stdio.h>
#include <string.h>

#include "extclib/crypto.h"

#define MAX_SIZE 32 // multiple 128 bit for AES block

void print_bytes(uint8_t * array, size_t length);

int main(void) {
    uint8_t in[BUFSIZ] = "1234567890abcdefghij";

    Crypto params = {
        .mode = CBC_MODE,
        .data = {
            .size = strlen(in),
            .in = in,
            .out = in,
        },
        .key = {
            .size = 128,
            .bytes = "AES_key_128_bits",
            .iv = "abcdefghijklmnop",
        },
    };

    print_bytes(in, MAX_SIZE);

    params.option = ENCRYPT_OPTION;
    aes_crypto(params);
    print_bytes(in, MAX_SIZE);

    params.option = DECRYPT_OPTION;
    aes_crypto(params);
    print_bytes(in, MAX_SIZE);
    return 0;
}

void print_bytes(uint8_t * array, size_t length) {
    printf("[ ");
    for (size_t i = 0; i < length; ++i)
        printf("%x ", array[i]);
    printf("]\n");
}
