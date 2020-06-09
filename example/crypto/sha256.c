#include <stdio.h>
#include <string.h>

#include "extclib/crypto.h"

void print_bytes(uint8_t * array, size_t length);

int main(void) {
    uint8_t in[BUFSIZ] = "1234567890abcdefghij";
    uint8_t hash[32] = {0};

    Crypto params = {
        .data = {
            .size = strlen(in),
            .in = in,
            .out = hash,
        },
    };

    sha256_crypto(params);
    print_bytes(hash, 32);
    return 0;
}

void print_bytes(uint8_t * array, size_t length) {
    printf("[ ");
    for (size_t i = 0; i < length; ++i)
        printf("%x ", array[i]);
    printf("]\n");
}
