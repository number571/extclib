#include <stddef.h>

#include "crypto.h"
#include "crypto/aes.h"

extern int8_t ecb_crypto(Crypto params) {
    int8_t code = 0;
    params.mode = ECB_MODE;
    switch(params.cipher){
        case AES_CIPHER: {
            const uint8_t bsize = 16;
            for (size_t i = 0; i < params.size && code == 0; i += bsize) {
                code = aes(&params);
                params.from += bsize;
                params.to   += bsize;
            }
        }
        break;
        default: return -1;
    }
    return code;
}

extern int8_t aes_crypto(Crypto params) {
    int8_t code = 0;
    params.cipher = AES_CIPHER;
    switch(params.mode) {
        case ECB_MODE:
            code = ecb_crypto(params);
        break;
        default: return -1;
    }
    return code;
}
