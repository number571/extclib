#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "extclib/crypto.h"

#define BUFFSIZE 20

void print_bytes(uint8_t * array, size_t length);

void init(void) {
    // WARNING: need get entropy from another source;
    uint8_t *seed = "init seed";
    size_t size = strlen(seed);
    srand_crypto((Context){
        .data = {
            .size = size,
            .in = seed,
        },
    });
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
