#include <stdint.h>
#include <string.h>

#include "extclib/io.h"
#include "extclib/crypto.h"

#define BUFFSIZE 64

int main(void) {
    uint8_t in[BUFFSIZE] = "abcdefghijklmnopqrstuvwxyz";
    uint8_t key[BUFFSIZE] = "1234567890";

    uint8_t hash[32] = {0};

    Context ctx = {
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

    sum_hmac256(ctx);
    printf_io("%$\n", hash, 32);
    return 0;
}
