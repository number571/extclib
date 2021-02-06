#ifndef EXTCLIB_SCRYPTO_H_
#define EXTCLIB_SCRYPTO_H_

extern void scrypto_rc4_init(char *key, int ksize);
extern void scrypto_rc4_generate(char *output, int size);

extern void scrypto_xor(char *output, char *key, int ksize, char *input, int size);

#endif /* EXTCLIB_SCRYPTO_H_ */
