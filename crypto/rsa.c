#include <stdint.h>

#include <openssl/rsa.h>

#include "rsa.h"
#include "context.h"

#include "_types.h"

typedef struct KeysRSA {
    size_t bsize;
    RSA *keys;
} KeysRSA;

static int8_t _rsa(option_t option, Context *ctx, KeysRSA *rsa);
static int8_t _oaep_crypto(option_t option, Context *ctx, KeysRSA *rsa);

extern KeysRSA *new_rsa(size_t size) {
    KeysRSA *rsa = (KeysRSA*)malloc(sizeof(KeysRSA));
    rsa->keys = RSA_new();
    rsa->bsize = size/8;
    BIGNUM *bn = BN_new();
    BN_set_word(bn, RSA_F4);
    int code = RSA_generate_key_ex(rsa->keys, size, bn, NULL);
    if (code != 1) {
        return NULL;
    }
    return rsa;
}

extern void free_rsa(KeysRSA *rsa) {
    RSA_free(rsa->keys);
    free(rsa);
}

extern int8_t encrypt_rsa(Context ctx, KeysRSA *rsa) {
    return _rsa(ENCRYPT_OPTION, &ctx, rsa);
}

extern int8_t decrypt_rsa(Context ctx, KeysRSA *rsa) {
    return _rsa(DECRYPT_OPTION, &ctx, rsa);
}

static int8_t _rsa(option_t option, Context *ctx, KeysRSA *rsa) {
    switch(ctx->mode) {
        case OAEP_MODE:
            return _oaep_crypto(option, ctx, rsa);
        break;
        default: return -1;
    }
}

static int8_t _oaep_crypto(option_t option, Context *ctx, KeysRSA *rsa) {
    int enclen = 0;
    switch(option) {
        case ENCRYPT_OPTION:
            enclen = RSA_public_encrypt(rsa->bsize-42, ctx->data.in, ctx->data.out, rsa->keys, RSA_PKCS1_OAEP_PADDING);
        break;
        case DECRYPT_OPTION:
            enclen = RSA_private_decrypt(rsa->bsize, ctx->data.in, ctx->data.out, rsa->keys, RSA_PKCS1_OAEP_PADDING);
        break;
    }
    if (enclen == -1) {
        return 1;
    }
    return 0;
}