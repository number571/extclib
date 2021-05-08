#include "extclib/type/bigint.h"
#include "extclib/crypto/akey.h"

#include <string.h>

static void _print_hex(unsigned char *b8, int size);

int main(void) {
	// 24-bit key
	akey_t *key = akey_new(24);
	int ksize = akey_size(key);

	unsigned char message[ksize];
	unsigned char decrypted[ksize];
	unsigned char encrypted[ksize*2];

	memset(message, 0, ksize);
	memcpy(message, "ABC", 3);

	// Message
	_print_hex(message, ksize);

	// Encrypted
	akey_encrypt(encrypted, key, message);
	_print_hex(encrypted, ksize*2);

	// Decrypted
	akey_decrypt(decrypted, key, encrypted);
	_print_hex(decrypted, ksize);

	akey_free(key);
	return 0;
}

static void _print_hex(unsigned char *b8, int size) {
	for (int i = 0; i < size; ++i) {
		printf("%02x", b8[i]);
	}
	printf("\n");
}
