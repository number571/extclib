#include <stdio.h>
#include <string.h>

#include "extclib/crypto.h"

static void _print_hex(unsigned char *b8, int size) {
	for (int i = 0; i < size; ++i) {
		printf("%02x", b8[i]);
	}
	printf("\n");
}

int main(void) {
	unsigned char hash[32];
	unsigned char msg[] = "hello, world!";
	
	crypto_hash(hash, msg, strlen((char*)msg));

	_print_hex(hash, 32);

	return 0;
}
