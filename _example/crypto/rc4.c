#include "extclib/crypto.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define KSIZE 32

static void print_bytes(uint8_t *bytes, int size);

int main(void) {
	uint8_t message[BUFSIZ] = "hello, world!";
	size_t  msglen = strlen((char*)message);

	uint8_t rnbytes[BUFSIZ];
	uint8_t mainkey[KSIZE];

	print_bytes(message, msglen);

	crypto_entropy(mainkey, KSIZE);
	crypto_srand(mainkey, KSIZE);
	crypto_rand(rnbytes, BUFSIZ);

	crypto_xor(message, rnbytes, BUFSIZ, message, msglen);
	print_bytes(message, msglen);

	crypto_xor(message, rnbytes, BUFSIZ, message, msglen);
	print_bytes(message, msglen);

	return 0;
}

static void print_bytes(uint8_t *bytes, int size) {
	printf("[ ");
	for (int i = 0; i < size; ++i) {
		printf("%d ", bytes[i]);
	}
	printf("]\n");
}
