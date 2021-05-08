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
	unsigned char msg[100] = "hello, world!";
	unsigned char key[] = "it's a key?";
	unsigned char iv[]  = "wow it's iv!!";

	int msglen = strlen((char*)msg);

	_print_hex(msg, msglen);

	crypto_encrypt(
		msg, 
		key, strlen((char*)key),
		iv, strlen((char*)iv),
		msg, strlen((char*)msg)
	);

	_print_hex(msg, msglen);

	crypto_encrypt(
		msg, 
		key, strlen((char*)key),
		iv, strlen((char*)iv),
		msg, strlen((char*)msg)
	);

	_print_hex(msg, msglen);

	return 0;
}
