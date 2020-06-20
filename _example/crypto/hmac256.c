#include <stdio.h>
#include <string.h>

#include "extclib/crypto.h"

void print_bytes(uint8_t * array, size_t length);

int main(void) {
    uint8_t in[BUFSIZ] = "abcdefghijklmnopqrstuvwxyz";
    uint8_t key[BUFSIZ] = "1234567890";

    uint8_t hash[32] = {0};

    Context params = {
        .data = {
            .size = strlen((char*)in),
            .in = in,
            .out = hash,
        },
        .desc = {
            .size = strlen((char*)key),
            .vec1 = key,
        },
    };

    hmac256_crypto(params);
    print_bytes(hash, 32);
    return 0;
}

void print_bytes(uint8_t * array, size_t length) {
    printf("[ ");
    for (size_t i = 0; i < length; ++i) {
        if (array[i] <= 0xF) {
            putchar('0');
        }
        printf("%x ", array[i]);
    }
    printf("]\n");
}
