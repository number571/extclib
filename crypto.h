#ifndef EXTCLIB_CRYPTO_H_
#define EXTCLIB_CRYPTO_H_

// Speck-OFB
extern void crypto_encrypt (
	unsigned char * output,
	const unsigned char * const key,
	int ksize,
	const unsigned char * const iv,
	int vsize,
	const unsigned char * const input, 
	int isize
);

// RAND(Speck-OFB)
extern void crypto_rand (
	unsigned char *output, 
	int size
);

// MDC-2(Speck)
extern void crypto_hash (
	unsigned char output[32],
	const unsigned char * const input, 
	int size
);

// Proof-Of-Work(MDC-2[Speck])
extern unsigned long long crypto_pow (
	unsigned char hash[32], 
	unsigned int diff
);

#endif /* EXTCLIB_CRYPTO_H_ */
