# extclib
> Extented C library. Version 0.5.0.

### Implemented
1. Net: tcp, http;
2. Type: bigint, hashtab, list, stack;
3. Crypto: rand, hash, encrypt;

### Compile library
```
$ make -C extclib/
> gcc -Wall -std=c99 -c crypto.c net.c net/http.c type/bigint.c type/hashtab.c type/list.c type/stack.c type/mini-gmp/mini-gmp.c type/mini-gmp/mini-mpq.c
> ld -r crypto.o net.o net/http.o type/bigint.o type/hashtab.o type/list.o type/stack.o type/mini-gmp/mini-gmp.o type/mini-gmp/mini-mpq.o -o extclib.o
$ cc main.c extclib/extclib.o -o main
```

### Example
```c
#include "extclib/crypto.h"
#include "extclib/type/bigint.h"

bigint_t *generate_prime(int bits) {
	const int bsize = bits/8;
	unsigned char buffer[bsize];

	bigint_t *num = bigint_new("");
	crypto_rand(buffer, bsize);
	bigint_load(num, buffer, bsize);

	while(!bigint_isprime(num)) {
		crypto_rand(buffer+(bsize-8), 8);
		bigint_load(num, buffer, bsize);
	}

	return num;
}

int main(void) {
	bigint_t *prime = generate_prime(2048);
	bigint_out(prime, stdout, 10);
	bigint_free(prime);
	return 0;
}
```
