#include <stdint.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include "_types.h"
#include "_macro_ctx.h"

static int8_t _rsa(option_t option, Context *ctx, RSA *rsa);
static int8_t _oaep_rsa(option_t option, Context *ctx, RSA *rsa);
static int8_t _sign_rsa(option_t option, Context *ctx, RSA *rsa);

extern RSA *new_rsa(size_t size) {
    RSA *rsa = RSA_new();
    BIGNUM *bn = BN_new();
    BN_set_word(bn, RSA_F4);
    int code = RSA_generate_key_ex(rsa, size, bn, NULL);
    if (code != 1) {
        return NULL;
    }
    return rsa;
}

extern void free_rsa(RSA *rsa) {
    RSA_free(rsa);
}

extern size_t size_rsa(RSA *rsa) {
    return RSA_size(rsa);
}

extern size_t sizeof_rsa(void) {
    return sizeof(RSA*);
}

extern RSA *pemin_rsa(FILE *stream) {
    RSA *rsa = PEM_read_RSAPrivateKey(stream, NULL, 0, NULL);
    if (rsa == NULL) {
        free(rsa);
        return NULL;
    }
    return rsa;
}

extern void pemout_rsa(RSA *rsa, FILE *stream, _Bool ispriv) {
    if (ispriv) {
        PEM_write_RSAPrivateKey(stream, rsa, NULL, NULL, 0, NULL, NULL);
        return;
    }
    PEM_write_RSAPublicKey(stream, rsa);
}

extern RSA *pemload_rsa(uint8_t *priv) {
    BIO *bio = BIO_new_mem_buf(priv, strlen((char*)priv));
    RSA *rsa = PEM_read_bio_RSAPrivateKey(bio, NULL, 0, NULL);
    BIO_free_all(bio);
    if (rsa == NULL) {
        return NULL;
    }
    return rsa;
}

extern void pemstore_rsa(RSA *rsa, uint8_t *priv, uint8_t *pub) {
    if (priv != NULL) {
        BIO *d = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPrivateKey(d, rsa, NULL, NULL, 0, NULL, NULL);
        size_t priv_len = BIO_pending(d);
        BIO_read(d, priv, priv_len);
        priv[priv_len] = '\0';
        BIO_free_all(d);
    }
    if (pub != NULL) {
        BIO *e = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(e, rsa);
        size_t pub_len = BIO_pending(e);
        BIO_read(e, pub, pub_len);
        pub[pub_len] = '\0';
        BIO_free_all(e);
    }
}

extern void pemsize_rsa(RSA *rsa, size_t *priv, size_t *pub) {
    if (priv != NULL) {
        BIO *d = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPrivateKey(d, rsa, NULL, NULL, 0, NULL, NULL);
        *priv = BIO_pending(d);
        BIO_free_all(d);
    }
    if (pub != NULL) {
        BIO *e = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(e, rsa);
        *pub = BIO_pending(e);
        BIO_free_all(e);
    }
}

extern int8_t encrypt_rsa(Context ctx, RSA *rsa) {
    return _rsa(ENCRYPT_OPTION, &ctx, rsa);
}

extern int8_t decrypt_rsa(Context ctx, RSA *rsa) {
    return _rsa(DECRYPT_OPTION, &ctx, rsa);
}

static int8_t _rsa(option_t option, Context *ctx, RSA *rsa) {
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

static int8_t _oaep_rsa(option_t option, Context *ctx, RSA *rsa) {
    int enclen = 0;
    switch(option) {
        case ENCRYPT_OPTION:
            enclen = RSA_public_encrypt(RSA_size(rsa)-42, ctx->data.in, ctx->data.out, rsa, RSA_PKCS1_OAEP_PADDING);
        break;
        case DECRYPT_OPTION:
            enclen = RSA_private_decrypt(RSA_size(rsa), ctx->data.in, ctx->data.out, rsa, RSA_PKCS1_OAEP_PADDING);
        break;
    }
    if (enclen == -1) {
        return 1;
    }
    return 0;
}

static int8_t _sign_rsa(option_t option, Context *ctx, RSA *rsa) {
    switch(option) {
        case ENCRYPT_OPTION: {
            EVP_MD_CTX *CTX = EVP_MD_CTX_new();
            EVP_PKEY *pub = EVP_PKEY_new();
            EVP_PKEY_assign_RSA(pub, rsa);
            if (EVP_DigestVerifyInit(CTX, NULL, EVP_sha256(), NULL, pub) != 1) {
                EVP_MD_CTX_free(CTX);
                return 1;
            }
            if (EVP_DigestVerifyUpdate(CTX, ctx->data.in, ctx->data.size) != 1) {
                EVP_MD_CTX_free(CTX);
                return 2;
            }
            if (EVP_DigestVerifyFinal(CTX, ctx->data.out, RSA_size(rsa)) != 1) {
                EVP_MD_CTX_free(CTX);
                return 3;
            }
            EVP_MD_CTX_free(CTX);
        }
        break;
        case DECRYPT_OPTION: {
            EVP_MD_CTX *CTX = EVP_MD_CTX_new();
            EVP_PKEY *priv  = EVP_PKEY_new();
            EVP_PKEY_assign_RSA(priv, rsa);
            size_t length = RSA_size(rsa);
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
