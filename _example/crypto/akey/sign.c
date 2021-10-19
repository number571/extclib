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

	unsigned char signedm[ksize*2];
	unsigned char message[] = "hello, world!";
	int msglen = strlen((char*)message);

	// Message
	_print_hex(message, msglen);

	// Signed
	akey_sign(prv, signedm, message, msglen);
	_print_hex(signedm, ksize*2);

	// Verify
	printf("%d\n", akey_verify(pub, signedm, message, msglen));

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
