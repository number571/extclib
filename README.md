# extclib
> Extented C library. Version 0.7.1.

### Implemented
1. Net: tcp, http;
2. Math: bigint, rational;
3. Type: hashtab, list, stack;
4. Crypto: rand, hash, encrypt, akey(encrypt, sign), proof;
5. Database: sqlite3;
6. Encoding: 8to64, 64to8, 8to32, 32to8, 8to16, 16to8;

### Compile library
```
$ make -C extclib/
> gcc -Wall -std=c99 -c crypto.c crypto/akey.c crypto/proof.c encoding.c database.c database/sqlite3.c net.c net/http.c math/bigint.c math/rational.c type/hashtab.c type/list.c type/stack.c math/bigint/mini-gmp.c math/bigint/mini-mpq.c
> ld -r crypto.o crypto/akey.o crypto/proof.o encoding.o database.o database/sqlite3.o net.o net/http.o math/bigint.o math/rational.o type/hashtab.o type/list.o type/stack.o math/bigint/mini-gmp.o math/bigint/mini-mpq.o -o extclib.o
// if linux
$ gcc -std=c99 -Wall -o main main.c extclib/extclib.o -lpthread -ldl 
// if windows
$ gcc -std=c99 -Wall -o main.exe main.c extclib/extclib.o -lws2_32
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
