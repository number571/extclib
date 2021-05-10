#ifndef EXTCLIB_CRYPTO_AKEY_H_
#define EXTCLIB_CRYPTO_AKEY_H_

#include "../type/bigint.h"

#define AKEY_PUBLIC   1
#define AKEY_PRIVATE -1

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
	unsigned char * output,
	const akey_t *key,
	const unsigned char * const input
);

// size of output = size(key)
extern int akey_decrypt (
	unsigned char * output,
	const akey_t *key,
	const unsigned char * const input
);

// // size of output = size(key)*2
// extern int akey_sign (
// 	unsigned char * output,
// 	const akey_t *key,
// 	const unsigned char * const input
// );

// // size of output = size(key)
// extern int akey_verify (
// 	unsigned char * output,
// 	const akey_t *key,
// 	const unsigned char * const input
// );

#endif /* EXTCLIB_CRYPTO_AKEY_H_ */
