#ifndef EXTCLIB_SCRYPTO_H_
#define EXTCLIB_SCRYPTO_H_

extern void scrypto_srand(const char *key, int ksize);
extern void scrypto_rand(char *output, int size);

extern int scrypto_entropy(char *output, int size);
extern void scrypto_xor(char *output, const char *key, int ksize, const char *input, int size);

#endif /* EXTCLIB_SCRYPTO_H_ */
