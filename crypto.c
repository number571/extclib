#include <string.h>

#include "crypto.h"
#include "crypto/aes.h"
#include "crypto/sha256.h"

#include "context.h"

typedef enum cipher_t {
    AES_CIPHER,
} cipher_t;

static void _rand_blocks_crypto(Context *ctx);
static void _increment_ctr(uint8_t *counter, size_t size);

static void _sha256_crypto(Context *ctx);
static int8_t _ecb_crypto(cipher_t cipher, option_t option, Context *ctx);
static int8_t _cbc_crypto(cipher_t cipher, option_t option, Context *ctx);

#define AES_KSIZE 32 // key size = 32 byte = 256 bit
#define AES_BSIZE 16 // block size = 16 byte = 128 bit

// rand = Fortuna (Practical Cryptography, Niels Ferguson & Bruce Schneier)
static _Bool srand_used = 0;
static uint8_t mainkey[AES_KSIZE] = {0};
static uint8_t counter[AES_BSIZE] = {0};

extern void srand_crypto(Context ctx) {
    ctx.data.out = mainkey;
    ctx.desc.size = AES_KSIZE;
    ctx.desc.vec1 = mainkey;
    hmac256_crypto(ctx);
    memset(counter, 0, AES_BSIZE);
    srand_used = 1;
}

extern int8_t rand_crypto(Context ctx) {
    const size_t maxvalue = (1 << 20); // MAX = 1MiB
    if (ctx.data.size >= maxvalue) {
        return 1;
    }
    if (srand_used == 0) {
        return 2;
    }
    // generate [ctx.data.size] bytes
    _rand_blocks_crypto(&ctx);
    // then update key
    ctx.data.size = AES_KSIZE;
    ctx.data.out = mainkey;
    _rand_blocks_crypto(&ctx);
    return 0;
}

extern int8_t aes_crypto(option_t option, Context ctx) {
    switch(ctx.mode) {
        case DEF_MODE: case ECB_MODE:
            return _ecb_crypto(AES_CIPHER, option, &ctx);
        break;
        case CBC_MODE:
            return _cbc_crypto(AES_CIPHER, option, &ctx);
        break;
    }
    return 0;
}

extern void hmac256_crypto(Context ctx) {
    const uint8_t hsize = 32; // sha256 size
    const uint8_t bsize = 64; // hmac block size RFC 2104
    const uint8_t ipad = 0x36;
    const uint8_t opad = 0x5c;

    Context temp_ctx;
    uint8_t keybuff[bsize];
    uint8_t ikeypad[bsize];
    uint8_t okeypad[bsize];
    uint8_t result[ctx.data.size+bsize+hsize];

    if (ctx.desc.size > bsize) {
        temp_ctx.data.size = ctx.desc.size;
        temp_ctx.data.in = ctx.desc.vec1;
        temp_ctx.data.out = keybuff;
        _sha256_crypto(&temp_ctx);
    }
    if (ctx.desc.size < bsize) {
        memcpy(keybuff, ctx.desc.vec1, ctx.desc.size);
        memset(keybuff + ctx.desc.size, 0, bsize - ctx.desc.size);
    }

    for (uint8_t i = 0; i < bsize; ++i) {
        ikeypad[i] = keybuff[i] ^ ipad;
        okeypad[i] = keybuff[i] ^ opad;
    }

    memcpy(result, ikeypad, bsize);
    memcpy(result + bsize, ctx.data.in, ctx.data.size);
    temp_ctx.data.size = ctx.data.size + bsize;
    temp_ctx.data.in = result;
    temp_ctx.data.out = keybuff;
    _sha256_crypto(&temp_ctx);

    memcpy(result, okeypad, bsize);
    memcpy(result + bsize, keybuff, hsize);
    temp_ctx.data.size = bsize + hsize;
    temp_ctx.data.in = result;
    temp_ctx.data.out = ctx.data.out;
    _sha256_crypto(&temp_ctx);
}

static void _rand_blocks_crypto(Context *ctx) {
    const size_t ksize = AES_KSIZE * 8; // 256 bit key
    uint32_t wkeys[60];
    uint8_t buffer[AES_BSIZE];
    aes_key_setup(mainkey, wkeys, ksize);
    for (size_t i = 0; i < ctx->data.size; i += AES_BSIZE) {
        aes_encrypt(counter, buffer, wkeys, ksize);
        _increment_ctr(counter, AES_BSIZE);
        if (i + AES_BSIZE > ctx->data.size) {
            memcpy(ctx->data.out + i, buffer, ctx->data.size - i);
            break;
        }
        memcpy(ctx->data.out + i, buffer, AES_BSIZE);
    }
}

static void _increment_ctr(uint8_t *ctr, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        ctr[i] += 1;
        if (ctr[i] != 0) {
            break;   
        }
    }
}

static void _sha256_crypto(Context *ctx) {
    SHA256_CTX shactx;
    sha256_init(&shactx);
    sha256_update(&shactx, ctx->data.in, ctx->data.size);
    sha256_final(&shactx, ctx->data.out);
}

static int8_t _ecb_crypto(cipher_t cipher, option_t option, Context *ctx) {
    switch(cipher) {
        case AES_CIPHER: {
            uint32_t wkeys[60]; 
            switch(ctx->desc.size){
                case 128: case 192: case 256: break;
                default: return 1;
            }
            aes_key_setup(ctx->desc.vec1, wkeys, ctx->desc.size);
            switch(option) {
                case ENCRYPT_OPTION:
                    for (size_t i = 0; i < ctx->data.size; i += AES_BSIZE) {
                        aes_encrypt(ctx->data.in + i, ctx->data.out + i, wkeys, ctx->desc.size);
                    }
                break;
                case DECRYPT_OPTION:
                    for (size_t i = 0; i < ctx->data.size; i += AES_BSIZE) {
                        aes_decrypt(ctx->data.in + i, ctx->data.out + i, wkeys, ctx->desc.size);
                    }
                break;
            }
        }
        break;
        default: return -1;
    }
    return 0;
}

static int8_t _cbc_crypto(cipher_t cipher, option_t option, Context *ctx) {
    switch(cipher) {
        case AES_CIPHER: {
            uint32_t wkeys[60];
            switch(ctx->desc.size){
                case 128: case 192: case 256: break;
                default: return 1;
            }
            aes_key_setup(ctx->desc.vec1, wkeys, ctx->desc.size);
            size_t dsize = ctx->data.size + (AES_BSIZE - ctx->data.size % AES_BSIZE);
            switch(option) {
                case ENCRYPT_OPTION:
                    aes_encrypt_cbc(ctx->data.in, dsize, ctx->data.out, wkeys, ctx->desc.size, ctx->desc.vec2);
                break;
                case DECRYPT_OPTION:
                    aes_decrypt_cbc(ctx->data.in, dsize, ctx->data.out, wkeys, ctx->desc.size, ctx->desc.vec2);
                break;
            }
        }
        break;
        default: return -1;
    }
    return 0;
}

