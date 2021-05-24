#ifndef EXTCLIB_CRYPTO_PROOF_H_
#define EXTCLIB_CRYPTO_PROOF_H_

// Proof-Of-Work(MDC-2[Speck])
extern unsigned long long proof_of_work (
	const unsigned char hash[32], 
	unsigned char diff
);

extern int proof_verify (
	const unsigned char hash[32],
	unsigned char diff,
	unsigned long long proof
);

#endif /* EXTCLIB_CRYPTO_PROOF_H_ */
