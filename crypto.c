#include "crypto.h"
#include "crypto/aes.h"
#include "crypto/sha256.h"

typedef enum cipher_t {
    AES_CIPHER,
} cipher_t;

static int8_t _ecb_crypto(cipher_t cipher, Crypto *params);
static int8_t _cbc_crypto(cipher_t cipher, Crypto *params);

extern int8_t aes_crypto(Crypto params) {
    switch(params.mode) {
        case DEF_MODE: case ECB_MODE:
            return _ecb_crypto(AES_CIPHER, &params);
        break;
        case CBC_MODE:
            return _cbc_crypto(AES_CIPHER, &params);
        break;
    }
    return 0;
}

extern void sha256_crypto(Crypto params) {
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, params.data.in, params.data.size);
    sha256_final(&ctx, params.data.out);
}

static int8_t _ecb_crypto(cipher_t cipher, Crypto *params) {
    switch(cipher) {
        case AES_CIPHER: {
            const uint8_t bsize = 16;
            uint32_t w[64]; 
            switch(params->key.size){
                case 128: case 192: case 256: break;
                default: return 1;
            }
            aes_key_setup(params->key.bytes, w, params->key.size);
            switch(params->option) {
                case ENCRYPT_OPTION:
                    for (size_t i = 0; i < params->data.size; i += bsize) {
                        aes_encrypt(params->data.in + i, params->data.out + i, w, params->key.size);
                    }
                break;
                case DECRYPT_OPTION:
                    for (size_t i = 0; i < params->data.size; i += bsize) {
                        aes_decrypt(params->data.in + i, params->data.out + i, w, params->key.size);
                    }
                break;
            }
        }
        break;
        default: return -1;
    }
    return 0;
}

static int8_t _cbc_crypto(cipher_t cipher, Crypto *params) {
    switch(cipher) {
        case AES_CIPHER: {
            const uint8_t bsize = 16;
            uint32_t w[64]; 
            switch(params->key.size){
                case 128: case 192: case 256: break;
                default: return 1;
            }
            aes_key_setup(params->key.bytes, w, params->key.size);
            params->data.size = params->data.size + (bsize - params->data.size % bsize);
            switch(params->option) {
                case ENCRYPT_OPTION:
                    aes_encrypt_cbc(params->data.in, params->data.size, params->data.out, w, params->key.size, params->key.iv);
                break;
                case DECRYPT_OPTION:
                    aes_decrypt_cbc(params->data.in, params->data.size, params->data.out, w, params->key.size, params->key.iv);
                break;
            }
        }
        break;
        default: return -1;
    }
    return 0;
}

