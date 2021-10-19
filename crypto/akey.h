#ifndef EXTCLIB_CRYPTO_AKEY_H_
#define EXTCLIB_CRYPTO_AKEY_H_

#include "../math/bigint.h"

#define AKEY_PUBLIC   1
#define AKEY_PRIVATE -1

// Elgamal encryption
typedef struct akey_t akey_t;

extern akey_t *akey_new(int bits);
extern void akey_free(akey_t *key);

extern int akey_size(akey_t *key);
extern akey_t *akey_set(
	int bytes, 
	int ktype,
	bigint_t *k,
	bigint_t *g, 
	bigint_t *p
);

extern bigint_t *akey_p(akey_t *key);
extern bigint_t *akey_g(akey_t *key);
extern bigint_t *akey_x(akey_t *key);
extern bigint_t *akey_y(akey_t *key);

// size of output = size(key)*2
extern int akey_encrypt (
	const akey_t *key,
	unsigned char * output,
	const unsigned char * const input
);

// size of output = size(key)
extern int akey_decrypt (
	const akey_t *key,
	unsigned char * output,
	const unsigned char * const input
);

// size of output = size(key)*2
extern int akey_sign (
	const akey_t *key,
	unsigned char * output,
	const unsigned char * const input,
	int isize
);

// if ret = 0 -> success
// else -> failed
extern int akey_verify (
	const akey_t *key,
	const unsigned char * const sign,
	const unsigned char * const input,
	int isize
);

#endif /* EXTCLIB_CRYPTO_AKEY_H_ */
