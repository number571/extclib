#ifndef EXTCLIB_CRYPTO_H_
#define EXTCLIB_CRYPTO_H_

extern void crypto_encrypt (
	unsigned char * output,
	const unsigned char * const key,
	int ksize,
	const unsigned char * const iv,
	int vsize,
	const unsigned char * const input, 
	int isize
);

extern void crypto_rand (
	unsigned char *output, 
	int size
);

extern void crypto_hash (
	unsigned char output[32],
	const unsigned char * const input, 
	int size
);

#endif /* EXTCLIB_CRYPTO_H_ */
