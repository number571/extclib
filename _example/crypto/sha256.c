#include <stdint.h>
#include <string.h>

#include "extclib/io.h"
#include "extclib/crypto.h"

#define BUFFSIZE 64

int main(void) {
    uint8_t in[BUFFSIZE] = "abcdefghijklmnopqrstuvwxyz";
    uint8_t hash[32] = {0};

    Context params = {
        .data = {
            .size = strlen((char*)in),
            .in = in,
            .out = hash,
        },
    };

    sum_sha256(params);
    printf_io("%$\n", hash, 32);
    return 0;
}
