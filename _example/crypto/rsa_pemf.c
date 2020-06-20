#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "extclib/io.h"
#include "extclib/crypto.h"

#define BITSSIZE 512
#define BYTESIZE BITSSIZE/8

void test_encrypt(KeysRSA *keys);

int main(void) {
    KeysRSA *keys = new_rsa(BITSSIZE);

    FILE *priv = fopen("private.pem", "w");
    if (priv == NULL) {
        return 1;
    }
    pemout_rsa(keys, priv, 1);
    fclose(priv);

    FILE *pub = fopen("public.pem", "w");
    if (pub == NULL) {
        return 2;
    }
    pemout_rsa(keys, pub, 0);
    fclose(pub);

    free_rsa(keys); 

    priv = fopen("private.pem", "r");
    if (priv == NULL) {
        return 3;
    }
    keys = pemin_rsa(priv);
    fclose(priv);

    test_encrypt(keys);

    free_rsa(keys); 
    return 0;
}

void test_encrypt(KeysRSA *keys) {
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
}
