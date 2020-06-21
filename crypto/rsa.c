#include <stdint.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include "rsa.h"
#include "../context.h"

#include "_types.h"

typedef struct KeysRSA {
    RSA *keys;
} KeysRSA;

static int8_t _rsa(option_t option, Context *ctx, KeysRSA *rsa);
static int8_t _oaep_rsa(option_t option, Context *ctx, KeysRSA *rsa);
static int8_t _sign_rsa(option_t option, Context *ctx, KeysRSA *rsa);

extern KeysRSA *new_rsa(size_t size) {
    KeysRSA *rsa = (KeysRSA*)malloc(sizeof(KeysRSA));
    rsa->keys = RSA_new();
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

extern size_t size_rsa(KeysRSA *rsa) {
    return RSA_size(rsa->keys);
}

extern size_t sizeof_rsa(void) {
    return sizeof(KeysRSA);
}

extern KeysRSA *pemin_rsa(FILE *stream) {
    KeysRSA *rsa = (KeysRSA*)malloc(sizeof(KeysRSA));
    rsa->keys = PEM_read_RSAPrivateKey(stream, NULL, 0, NULL);
    if (rsa->keys == NULL) {
        free(rsa);
        return NULL;
    }
    return rsa;
}

extern void pemout_rsa(KeysRSA *rsa, FILE *stream, _Bool ispriv) {
    if (ispriv) {
        PEM_write_RSAPrivateKey(stream, rsa->keys, NULL, NULL, 0, NULL, NULL);
        return;
    }
    PEM_write_RSAPublicKey(stream, rsa->keys);
}

extern KeysRSA *pemload_rsa(uint8_t *priv) {
    BIO *bio = BIO_new_mem_buf(priv, strlen((char*)priv));
    KeysRSA *rsa = (KeysRSA*)malloc(sizeof(KeysRSA));
    rsa->keys = PEM_read_bio_RSAPrivateKey(bio, NULL, 0, NULL);
    if (rsa->keys == NULL) {
        BIO_free_all(bio);
        free(rsa);
        return NULL;
    }
    BIO_free_all(bio);
    return rsa;
}

extern void pemstore_rsa(KeysRSA *rsa, uint8_t *priv, uint8_t *pub) {
    if (priv != NULL) {
        BIO *d = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPrivateKey(d, rsa->keys, NULL, NULL, 0, NULL, NULL);
        size_t priv_len = BIO_pending(d);
        BIO_read(d, priv, priv_len);
        priv[priv_len] = '\0';
        BIO_free_all(d);
    }
    if (pub != NULL) {
        BIO *e = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(e, rsa->keys);
        size_t pub_len = BIO_pending(e);
        BIO_read(e, pub, pub_len);
        pub[pub_len] = '\0';
        BIO_free_all(e);
    }
}

extern void pemsize_rsa(KeysRSA *rsa, size_t *priv, size_t *pub) {
    if (priv != NULL) {
        BIO *d = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPrivateKey(d, rsa->keys, NULL, NULL, 0, NULL, NULL);
        *priv = BIO_pending(d);
        BIO_free_all(d);
    }
    if (pub != NULL) {
        BIO *e = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(e, rsa->keys);
        *pub = BIO_pending(e);
        BIO_free_all(e);
    }
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
            return _oaep_rsa(option, ctx, rsa);
        break;
        case SIGN_MODE:
            return _sign_rsa(option, ctx, rsa);
        break;
        default: return -1;
    }
}

static int8_t _oaep_rsa(option_t option, Context *ctx, KeysRSA *rsa) {
    int enclen = 0;
    switch(option) {
        case ENCRYPT_OPTION:
            enclen = RSA_public_encrypt(RSA_size(rsa->keys)-42, ctx->data.in, ctx->data.out, rsa->keys, RSA_PKCS1_OAEP_PADDING);
        break;
        case DECRYPT_OPTION:
            enclen = RSA_private_decrypt(RSA_size(rsa->keys), ctx->data.in, ctx->data.out, rsa->keys, RSA_PKCS1_OAEP_PADDING);
        break;
    }
    if (enclen == -1) {
        return 1;
    }
    return 0;
}

static int8_t _sign_rsa(option_t option, Context *ctx, KeysRSA *rsa) {
    switch(option) {
        case ENCRYPT_OPTION: {
            EVP_MD_CTX *CTX = EVP_MD_CTX_new();
            EVP_PKEY *pub = EVP_PKEY_new();
            EVP_PKEY_assign_RSA(pub, rsa->keys);
            if (EVP_DigestVerifyInit(CTX, NULL, EVP_sha256(), NULL, pub) != 1) {
                EVP_MD_CTX_free(CTX);
                return 1;
            }
            if (EVP_DigestVerifyUpdate(CTX, ctx->data.in, ctx->data.size) != 1) {
                EVP_MD_CTX_free(CTX);
                return 2;
            }
            if (EVP_DigestVerifyFinal(CTX, ctx->data.out, RSA_size(rsa->keys)) != 1) {
                EVP_MD_CTX_free(CTX);
                return 3;
            }
            EVP_MD_CTX_free(CTX);
        }
        break;
        case DECRYPT_OPTION: {
            EVP_MD_CTX *CTX = EVP_MD_CTX_new();
            EVP_PKEY *priv  = EVP_PKEY_new();
            EVP_PKEY_assign_RSA(priv, rsa->keys);
            size_t length = RSA_size(rsa->keys);
            if (EVP_DigestSignInit(CTX, NULL, EVP_sha256(), NULL, priv) != 1) {
                EVP_MD_CTX_free(CTX);
                return 1;
            }
            if (EVP_DigestSignUpdate(CTX, ctx->data.in, ctx->data.size) != 1) {
                EVP_MD_CTX_free(CTX);
                return 2;
            }
            if (EVP_DigestSignFinal(CTX, ctx->data.out, &length) != 1) {
                EVP_MD_CTX_free(CTX);
                return 3;
            }
            EVP_MD_CTX_free(CTX);
        }
        break;
    }
    return 0;
}
