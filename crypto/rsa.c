#include <stdint.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include "rsa.h"

#include "_types.h"
#include "_macro_ctx.h"

static int8_t _rsa(option_t option, Context *ctx, RSA_e *rsa);
static int8_t _oaep_rsa(option_t option, Context *ctx, RSA_e *rsa);
static int8_t _sign_rsa(option_t option, Context *ctx, RSA_e *rsa);

extern RSA_e *new_rsa(size_t size) {
    RSA_e *rsa = (RSA_e*)RSA_new();
    BIGNUM *bn = BN_new();
    BN_set_word(bn, RSA_F4);
    int code = RSA_generate_key_ex((RSA*)rsa, size, bn, NULL);
    if (code != 1) {
        return NULL;
    }
    return rsa;
}

extern void free_rsa(RSA_e *rsa) {
    RSA_free((RSA*)rsa);
}

extern size_t size_rsa(RSA_e *rsa) {
    return RSA_size((RSA*)rsa);
}

extern RSA_e *pemin_rsa(FILE *stream) {
    RSA_e *rsa = (RSA_e*)PEM_read_RSAPrivateKey(stream, NULL, 0, NULL);
    if (rsa == NULL) {
        return NULL;
    }
    return rsa;
}

extern void pemout_rsa(RSA_e *rsa, FILE *stream, _Bool ispriv) {
    if (ispriv) {
        PEM_write_RSAPrivateKey(stream, (RSA*)rsa, NULL, NULL, 0, NULL, NULL);
        return;
    }
    PEM_write_RSAPublicKey(stream, (RSA*)rsa);
}

extern RSA_e *pemload_rsa(uint8_t *priv) {
    BIO *bio = BIO_new_mem_buf(priv, strlen((char*)priv));
    RSA_e *rsa = (RSA_e*)PEM_read_bio_RSAPrivateKey(bio, NULL, 0, NULL);
    BIO_free_all(bio);
    if (rsa == NULL) {
        return NULL;
    }
    return rsa;
}

extern void pemstore_rsa(RSA_e *rsa, uint8_t *priv, uint8_t *pub) {
    if (priv != NULL) {
        BIO *d = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPrivateKey(d, (RSA*)rsa, NULL, NULL, 0, NULL, NULL);
        size_t priv_len = BIO_pending(d);
        BIO_read(d, priv, priv_len);
        priv[priv_len] = '\0';
        BIO_free_all(d);
    }
    if (pub != NULL) {
        BIO *e = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(e, (RSA*)rsa);
        size_t pub_len = BIO_pending(e);
        BIO_read(e, pub, pub_len);
        pub[pub_len] = '\0';
        BIO_free_all(e);
    }
}

extern void pemsize_rsa(RSA_e *rsa, size_t *priv, size_t *pub) {
    if (priv != NULL) {
        BIO *d = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPrivateKey(d, (RSA*)rsa, NULL, NULL, 0, NULL, NULL);
        *priv = BIO_pending(d);
        BIO_free_all(d);
    }
    if (pub != NULL) {
        BIO *e = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(e, (RSA*)rsa);
        *pub = BIO_pending(e);
        BIO_free_all(e);
    }
}

extern int8_t encrypt_rsa(Context ctx, RSA_e *rsa) {
    return _rsa(ENCRYPT_OPTION, &ctx, rsa);
}

extern int8_t decrypt_rsa(Context ctx, RSA_e *rsa) {
    return _rsa(DECRYPT_OPTION, &ctx, rsa);
}

static int8_t _rsa(option_t option, Context *ctx, RSA_e *rsa) {
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

static int8_t _oaep_rsa(option_t option, Context *ctx, RSA_e *rsa) {
    int enclen = 0;
    switch(option) {
        case ENCRYPT_OPTION:
            enclen = RSA_public_encrypt(size_rsa(rsa)-42, ctx->data.in, ctx->data.out, (RSA*)rsa, RSA_PKCS1_OAEP_PADDING);
        break;
        case DECRYPT_OPTION:
            enclen = RSA_private_decrypt(size_rsa(rsa), ctx->data.in, ctx->data.out, (RSA*)rsa, RSA_PKCS1_OAEP_PADDING);
        break;
    }
    if (enclen == -1) {
        return 1;
    }
    return 0;
}

static int8_t _sign_rsa(option_t option, Context *ctx, RSA_e *rsa) {
    switch(option) {
        case ENCRYPT_OPTION: {
            EVP_MD_CTX *CTX = EVP_MD_CTX_new();
            EVP_PKEY *pub = EVP_PKEY_new();
            EVP_PKEY_assign_RSA(pub, (RSA*)rsa);
            if (EVP_DigestVerifyInit(CTX, NULL, EVP_sha256(), NULL, pub) != 1) {
                EVP_MD_CTX_free(CTX);
                return 1;
            }
            if (EVP_DigestVerifyUpdate(CTX, ctx->data.in, ctx->data.size) != 1) {
                EVP_MD_CTX_free(CTX);
                return 2;
            }
            if (EVP_DigestVerifyFinal(CTX, ctx->data.out, size_rsa(rsa)) != 1) {
                EVP_MD_CTX_free(CTX);
                return 3;
            }
            EVP_MD_CTX_free(CTX);
        }
        break;
        case DECRYPT_OPTION: {
            EVP_MD_CTX *CTX = EVP_MD_CTX_new();
            EVP_PKEY *priv  = EVP_PKEY_new();
            EVP_PKEY_assign_RSA(priv, (RSA*)rsa);
            size_t length = size_rsa(rsa);
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
