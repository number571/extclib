#ifndef EXTCLIB_CRYPTO_PACKAGE_H_
#define EXTCLIB_CRYPTO_PACKAGE_H_

#include "akey.h"

// 2048-bit key
#define PACKAGE_AKEYSIZ 256

typedef struct package_t package_t;

extern package_t *package_new(unsigned char title[32], unsigned char data[512]);
extern void package_free(package_t *pack);

extern int package_size(void);
extern void *package_encode(package_t *pack);
extern package_t *package_decode(void *pack);

extern int package_encrypt(package_t *pack, akey_t *sender, akey_t *receiver, int diff);
extern int package_decrypt(package_t *pack, akey_t *receiver, int diff);

extern akey_t *package_sender(package_t *pack);
extern unsigned char *package_title(package_t *pack);
extern unsigned char *package_data(package_t *pack);

#endif /* EXTCLIB_CRYPTO_PACKAGE_H_ */
