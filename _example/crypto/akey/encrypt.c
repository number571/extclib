#include "extclib/math/bigint.h"
#include "extclib/crypto/akey.h"

#include <string.h>

static void _print_hex(unsigned char *b8, int size);

int main(void) {
	// 24-bit key
	akey_t *key = akey_new(3);
	int ksize = akey_size(key);

	akey_t *prv = akey_set(
		ksize, 
		AKEY_PRIVATE,
		akey_x(key),
		akey_g(key),
		akey_p(key)
	);

	akey_t *pub = akey_set(
		ksize, 
		AKEY_PUBLIC,
		akey_y(key),
		akey_g(key),
		akey_p(key)
	);

	unsigned char message[ksize];
	unsigned char decrypted[ksize];
	unsigned char encrypted[ksize*2];

	memset(message, 0, ksize);
	memcpy(message, "ABC", 3);

	// Message
	_print_hex(message, ksize);

	// Encrypted
	akey_encrypt(pub, encrypted, message);
	_print_hex(encrypted, ksize*2);

	// Decrypted
	akey_decrypt(prv, decrypted, encrypted);
	_print_hex(decrypted, ksize);

	akey_free(key);
	akey_free(pub);
	akey_free(prv);

	return 0;
}

static void _print_hex(unsigned char *b8, int size) {
	for (int i = 0; i < size; ++i) {
		printf("%02x", b8[i]);
	}
	printf("\n");
}
