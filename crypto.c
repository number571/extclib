#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <openssl/aes.h>
#include <openssl/sha.h>
#include <openssl/rand.h>

#include "crypto.h"
#include "context.h"

#define AES_KSIZE 32 // key size = 32 byte = 256 bit
#define AES_BSIZE 16 // block size = 16 byte = 128 bit

typedef enum cipher_t {
    AES_CIPHER,
    RSA_CIPHER,
} cipher_t;

static void _sha256_crypto(Context *ctx);
// static int8_t _oaep_crypto(cipher_t cipher, option_t option, Context *ctx);
static int8_t _ecb_crypto(cipher_t cipher, option_t option, Context *ctx);
static int8_t _cbc_crypto(cipher_t cipher, option_t option, Context *ctx);

// extern int8_t genkeys_crypto(size_t size, Context ctx) {
//     return 0;
// }

// extern int8_t rsa_crypto(option_t option, Context ctx) {
//     switch(ctx.mode) {
//         case OAEP_MODE:
//             return _oaep_crypto(RSA_CIPHER, option, &ctx);
//         break;
//         default: return -1;
//     }
// }

extern int8_t aes_crypto(option_t option, Context ctx) {
    switch(ctx.mode) {
        case ECB_MODE:
            return _ecb_crypto(AES_CIPHER, option, &ctx);
        break;
        case CBC_MODE:
            return _cbc_crypto(AES_CIPHER, option, &ctx);
        break;
        default: return -1;
    }
}

extern void sha256_crypto(Context ctx) {
    _sha256_crypto(&ctx);
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

extern int8_t rand_crypto(Context ctx) {
    _Bool code = RAND_bytes(ctx.data.out, ctx.data.size) == 1;
    return !code;
}

static void _sha256_crypto(Context *ctx) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, ctx->data.in, ctx->data.size);
    SHA256_Final(ctx->data.out, &sha256);
}

// static int8_t _oaep_crypto(cipher_t cipher, option_t option, Context *ctx) {
//     switch(cipher) {
//         case RSA_CIPHER:
//             switch(option) {
//                 case ENCRYPT_OPTION:
//                 break;
//                 case DECRYPT_OPTION:
//                 break;
//             }
//         break;
//         default: return 2;
//     }
//     return 0;
// }

static int8_t _ecb_crypto(cipher_t cipher, option_t option, Context *ctx) {
    switch(cipher) {
        case AES_CIPHER: {
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
        }
        break;
        default: return 2;
    }
    return 0;
}

static int8_t _cbc_crypto(cipher_t cipher, option_t option, Context *ctx) {
    switch(cipher) {
        case AES_CIPHER: {
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
        }
        break;
        default: return 2;
    }
    return 0;
}
