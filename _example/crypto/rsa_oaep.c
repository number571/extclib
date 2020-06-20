#include <stdint.h>
#include <string.h>

#include "extclib/io.h"
#include "extclib/crypto.h"

#define BITSSIZE 512
#define BYTESIZE BITSSIZE/8

int main(void) {
    KeysRSA *keys = new_rsa(BITSSIZE);
    uint8_t in[BYTESIZE] = "hello, world!";

    Context ctx = {
        .mode = OAEP_MODE,
        .data = {
            .in = in,
            .out = in,
        },
    };

    printf_io("%$\n", in, BYTESIZE);

    encrypt_rsa(ctx, keys);
    printf_io("%$\n", in, BYTESIZE);

    decrypt_rsa(ctx, keys);
    printf_io("%$\n", in, BYTESIZE);

    free_rsa(keys);    
    return 0;
}
