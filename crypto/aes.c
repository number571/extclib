#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <openssl/aes.h>

#include "aes.h"
#include "context.h"

#include "_types.h"

#define AES_BSIZE 16 // block size = 16 byte

static int8_t _aes(option_t option, Context *ctx);
static int8_t _ecb_crypto(option_t option, Context *ctx);
static int8_t _cbc_crypto(option_t option, Context *ctx);

extern int8_t encrypt_aes(Context ctx) {
	return _aes(ENCRYPT_OPTION, &ctx);
}

extern int8_t decrypt_aes(Context ctx) {
	return _aes(DECRYPT_OPTION, &ctx);
}

static int8_t _aes(option_t option, Context *ctx) {
    switch(ctx->mode) {
        case ECB_MODE:
            return _ecb_crypto(option, ctx);
        break;
        case CBC_MODE:
            return _cbc_crypto(option, ctx);
        break;
        default: return -1;
    }
}

static int8_t _ecb_crypto(option_t option, Context *ctx) {
    AES_KEY wkeys;
    switch(ctx->desc.size){
        case 128: case 192: case 256: break;
        default: return 1;
    }
    switch(option) {
        case ENCRYPT_OPTION:
            AES_set_encrypt_key(ctx->desc.vec1, ctx->desc.size, &wkeys);
            for (size_t i = 0; i < ctx->data.size; i += AES_BSIZE) {
                AES_ecb_encrypt(ctx->data.in + i, ctx->data.out + i, &wkeys, AES_ENCRYPT);
            }
        break;
        case DECRYPT_OPTION:
            AES_set_decrypt_key(ctx->desc.vec1, ctx->desc.size, &wkeys);
            for (size_t i = 0; i < ctx->data.size; i += AES_BSIZE) {
                AES_ecb_encrypt(ctx->data.in + i, ctx->data.out + i, &wkeys, AES_DECRYPT);
            }
        break;
    }
    return 0;
}

static int8_t _cbc_crypto(option_t option, Context *ctx) {
    AES_KEY wkeys;
    uint8_t iv[AES_BSIZE];
    switch(ctx->desc.size){
        case 128: case 192: case 256: break;
        default: return 1;
    }
    memcpy(iv, ctx->desc.vec2, AES_BSIZE);
    size_t dsize = ctx->data.size + ((AES_BSIZE - ctx->data.size) % AES_BSIZE);
    switch(option) {
        case ENCRYPT_OPTION:
            AES_set_encrypt_key(ctx->desc.vec1, ctx->desc.size, &wkeys);
            AES_cbc_encrypt(ctx->data.in, ctx->data.out, dsize, &wkeys, iv, AES_ENCRYPT);
        break;
        case DECRYPT_OPTION:
            AES_set_decrypt_key(ctx->desc.vec1, ctx->desc.size, &wkeys);
            AES_cbc_encrypt(ctx->data.in, ctx->data.out, dsize, &wkeys, iv, AES_DECRYPT);
        break;
    }
    return 0;
}
