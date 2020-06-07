#include <stddef.h>
#include <string.h>

#include "crypto.h"
#include "crypto/aes.h"

typedef enum cipher_t {
    AES_CIPHER,
} cipher_t;

static int8_t _ecb_crypto(cipher_t cipher, Crypto *params);
static int8_t _cbc_crypto(cipher_t cipher, Crypto *params);

extern int8_t aes_crypto(Crypto params) {
    int8_t code = 0;
    switch(params.mode) {
        case DEF_MODE: case ECB_MODE:
            code = _ecb_crypto(AES_CIPHER, &params);
        break;
        case CBC_MODE:
            code = _cbc_crypto(AES_CIPHER, &params);
        break;
        default: return -1;
    }
    return code;
}

static int8_t _ecb_crypto(cipher_t cipher, Crypto *params) {
    int8_t code = 0;
    switch(cipher){
        case AES_CIPHER: {
            const uint8_t bsize = 16;
            for (uint32_t i = 0; i < params->data.size && code == 0; i += bsize) {
                code = aes(params);
                params->data.bytes += bsize;
            }
        }
        break;
        default: return -2;
    }
    return code;
}

static int8_t _cbc_crypto(cipher_t cipher, Crypto *params) {
    int8_t code = 0;
    switch(cipher){
        case AES_CIPHER: {
            const uint8_t bsize = 16;
            switch(params->option) {
                case ENCRYPT_OPTION: {
                    for (uint32_t i = 0; i < params->data.size && code == 0; i += bsize) {
                        for (uint8_t j = 0; j < bsize; ++j) {
                            params->data.bytes[j] ^= params->key.iv[j];
                        }
                        code = aes(params);
                        params->key.iv = params->data.bytes;
                        params->data.bytes += bsize;
                    }
                }
                break;
                case DECRYPT_OPTION: {
                    uint8_t ivect[bsize];
                    uint8_t block[bsize];
                    memcpy(ivect, params->key.iv, bsize);
                    for (uint32_t i = 0; i < params->data.size && code == 0; i += bsize) {
                        memcpy(block, params->data.bytes, bsize);
                        code = aes(params);
                        for (uint8_t j = 0; j < bsize; ++j) {
                            params->data.bytes[j] ^= ivect[j];
                        }
                        params->data.bytes += bsize;
                        memcpy(ivect, block, bsize);
                    }
                }
                break;
            }
        }
        break;
        default: return -2;
    }
    return code;
}
