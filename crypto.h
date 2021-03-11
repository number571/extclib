#ifndef EXTCLIB_CRYPTO_H_
#define EXTCLIB_CRYPTO_H_

/*
	todo:
	1. Hash function SHA256
	2. Asymmetric function El-Gamal
*/

extern int crypto_entropy(unsigned char *output, int size);

extern void crypto_srand(const unsigned char *key, int ksize);
extern void crypto_rand(unsigned char *output, int size);

extern void crypto_xor(
	unsigned char *output, 
	const unsigned char *key, 
	int ksize, 
	const unsigned char *input, 
	int size
);

#endif /* EXTCLIB_CRYPTO_H_ */
