#ifndef EXTCLIB_CRYPTO_H_
#define EXTCLIB_CRYPTO_H_

#include <stddef.h>
#include <stdint.h>

typedef enum option_t {
    ENCRYPT_OPTION,
    DECRYPT_OPTION,
} option_t;

typedef enum encmode_t {
    DEF_MODE,
    ECB_MODE,
    CBC_MODE,
} encmode_t;

typedef struct Crypto {
    encmode_t mode;
    option_t option;
    struct {
        uint32_t size; // bytes
        uint8_t *in;
        uint8_t *out;
    } data;
    struct {
        uint16_t size; // bits
        uint8_t *bytes;
        uint8_t *iv; // init vector
    } key;
} Crypto;

// CIPHERS
extern int8_t aes_crypto(Crypto params);

// HASH FUNCTIONS
extern void sha256_crypto(Crypto params);

#endif /* EXTCLIB_CRYPTO_H_ */
