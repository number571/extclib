#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "extclib/crypto.h"

#define SEEDSIZE 32
#define BUFFSIZE 20

void print_bytes(uint8_t * array, size_t length);

void init(void) {
    uint8_t seed[SEEDSIZE] = {0};
    Context ctx = {
        .data = {
            .size = SEEDSIZE,
            .out = seed,
            .in = seed,
        },
    };
    entropy_crypto(ctx);
    srand_crypto(ctx);
}

int main(void) {
    init(); // call srand
    uint8_t buffer[BUFFSIZE] = {0};
    Context ctx = {
        .data = {
            .size = BUFFSIZE,
            .out = buffer,
        },
    };
    for (size_t i = 0; i < 10; ++i) {
        rand_crypto(ctx);
        print_bytes(buffer, BUFFSIZE);
    }
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
