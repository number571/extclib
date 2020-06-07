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
        uint32_t size; // in bytes
        uint8_t *to;
        uint8_t *from;
    } data;
    struct {
        uint16_t size; // in bits
        uint8_t *iv; // init vector
        uint8_t *bytes;
    } key;
} Crypto;

// CIPHERS
extern int8_t aes_crypto(Crypto params);

#endif /* EXTCLIB_CRYPTO_H_ */
