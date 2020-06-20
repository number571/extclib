#include <stdint.h>
#include <string.h>

#include "extclib/io.h"
#include "extclib/crypto.h"

#define BLCKSIZE 16
#define BUFFSIZE BLCKSIZE*2

int main(void) {
    uint8_t in[BUFFSIZE] = "1234567890abcdefghij";

    Context ctx = {
        .mode = ECB_MODE,
        .data = {
            .size = strlen((char*)in),
            .in = in,
            .out = in,
        },
        .desc = {
            .size = 128,
            .vec1 = (uint8_t*)"AES_key_128_bits", // key
        },
    };

    printf_io("%$\n", in, BUFFSIZE);

    encrypt_aes(ctx);
    printf_io("%$\n", in, BUFFSIZE);

    decrypt_aes(ctx);
    printf_io("%$\n", in, BUFFSIZE);

    return 0;
}
