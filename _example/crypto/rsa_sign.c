#include <stdint.h>
#include <string.h>

#include "extclib/io.h"
#include "extclib/crypto.h"

#define BITSSIZE 512
#define BYTESIZE BITSSIZE/8

int main(void) {
    RSA *keys = new_rsa(BITSSIZE);
    uint8_t in[BYTESIZE] = "hello, world!";
    uint8_t out[BYTESIZE] = {0};

    Context ctx = {
        .mode = SIGN_MODE,
        .data = {
            .in = in,
            .out = out,
            .size = strlen((char*)in),
        },
    };

    // sign(hash(message)) = D(H(m))
    decrypt_rsa(ctx, keys);
    printf_io("%$\n", out, BYTESIZE);

    // verify(sign(hash(message))) = E(D(H(m))) = H(m)
    if (encrypt_rsa(ctx, keys) == 0) {
        printf("verify success\n");
    } else {
        printf("verify failed\n");
    }

    free_rsa(keys);    
    return 0;
}
