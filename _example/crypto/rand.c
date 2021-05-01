#include <stdio.h>

#include "extclib/crypto.h"

static void _print_hex(unsigned char *b8, int size) {
	for (int i = 0; i < size; ++i) {
		printf("%02x", b8[i]);
	}
	printf("\n");
}

int main(void) {
	unsigned char output[32];
	
	crypto_rand(output, 32);

	_print_hex(output, 32);

	return 0;
}
