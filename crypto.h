#ifndef EXTCLIB_CRYPTO_H_
#define EXTCLIB_CRYPTO_H_

#include <stddef.h>
#include <stdint.h>

typedef enum option_t {
    ENCRYPT_OPTION,
    DECRYPT_OPTION,
} option_t;

typedef enum cipher_t {
    AES_CIPHER,
} cipher_t;

typedef struct Crypto {
    size_t size; // in bytes
    uint8_t *iv; // init vector
    uint8_t *to;
    uint8_t *from;
    uint8_t *key;
    cipher_t cipher;
    option_t option; 
    uint8_t keysize; // in bytes
} Crypto;

// MODES
extern int8_t ecb_crypto(Crypto params);

#endif /* EXTCLIB_CRYPTO_H_ */
