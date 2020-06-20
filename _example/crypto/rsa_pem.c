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

    size_t priv_len, pub_len;
    pemsize_rsa(keys, &priv_len, &pub_len);

    printf("Priv: %ld\nPub: %ld\n", priv_len, pub_len);

    uint8_t priv[priv_len];
    uint8_t pub[pub_len];

    pemstore_rsa(keys, priv, pub);

    printf("\n%s\n%s\n", priv, pub);
    free_rsa(keys);    

    keys = pemload_rsa(priv);
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
