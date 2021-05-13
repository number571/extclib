# extclib
> Extented C library. Version 0.6.1.

### Implemented
1. Net: tcp, http;
2. Type: bigint, hashtab, list, stack;
3. Crypto: rand, hash, encrypt, akey(encrypt, sign);

### Compile library
```
$ make -C extclib/
> gcc -Wall -std=c99 -c crypto.c crypto/akey.c net.c net/http.c type/bigint.c type/hashtab.c type/list.c type/stack.c \
	type/mini-gmp/mini-gmp.c type/mini-gmp/mini-mpq.c
> ld -r crypto.o crypto/akey.o net.o net/http.o type/bigint.o type/hashtab.o type/list.o type/stack.o \
	type/mini-gmp/mini-gmp.o type/mini-gmp/mini-mpq.o -o extclib.o
$ cc main.c extclib/extclib.o -o main
```

### Example
```c
#include "extclib/type/bigint.h"
#include "extclib/crypto/akey.h"

#include <string.h>

static void _print_hex(unsigned char *b8, int size) {
	for (int i = 0; i < size; ++i) {
		printf("%02x", b8[i]);
	}
	printf("\n");
}

int main(void) {
	// 24-bit key
	akey_t *key = akey_new(3);
	int ksize = akey_size(key);

	unsigned char message[ksize];
	unsigned char decrypted[ksize];
	unsigned char encrypted[ksize*2];

	memset(message, 0, ksize);
	memcpy(message, "ABC", 3);

	// Message
	_print_hex(message, ksize);

	// Encrypted
	akey_encrypt(key, encrypted, message);
	_print_hex(encrypted, ksize*2);

	// Decrypted
	akey_decrypt(key, decrypted, encrypted);
	_print_hex(decrypted, ksize);

	akey_free(key);
	return 0;
}
```
