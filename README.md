# extclib
> Extended C library.

### Implemented:
1. Types: Array, BigInt, Dynamic, HashTab, List, Tree;
2. Crypto: AES[ECB, CBC], RSA[OAEP, SIGN], SHA256, HMAC[SHA256], RAND;
3. Encoding: Base64;
3. Net: Linux[arpa/inet.h], Windows[winsock2.h];

### Used libraries:
1. gmp: [gmplib.org/](https://gmplib.org/);
2. openssl: [openssl.org](https://www.openssl.org/);

### Compile library:
```
/* install 'gmp', 'openssl' libraries before compilation */
$ make -C extclib/

/* linux compile: */
$ cc main.c extclib/extclib.o -o main -lgmp -lcrypto

/* windows compile: */
$ cc main.c extclib/extclib.o -o main -lgmp -lcrypto -lws2_32
```

### Usage restrictions:
1. Recursive hash tables, trees, lists, arrays are not allowed. Cross-recursive is dangerous;
2. Values (and/or keys) in hash tables, trees, lists, arrays are freed up automatically when deleting or replacing an object's value;

### Example (IO, Types):
```c
#include "extclib/io.h"
#include "extclib/types.h"

// gcc main.c extclib/extclib.o -o main

int main(void) {
    HashTab *hashtab = new_hashtab(250, CHARS_TYPE, BIGINT_TYPE);

    set_hashtab(hashtab, chars("varX"), bigint(new_bigint("872138712637512787387124821738712648712736128749182")));
    set_hashtab(hashtab, chars("varY"), bigint(new_bigint("675346126835124712346172467268375128731")));
    set_hashtab(hashtab, chars("result"), bigint(new_bigint("")));

    // result <- varX * varY
    mul_bigint(
        get_hashtab(hashtab, chars("result")).bigint, 
        get_hashtab(hashtab, chars("varX")).bigint,
        get_hashtab(hashtab, chars("varY")).bigint
    );

    printf_io("(%B * %B = %B)\n", 
        get_hashtab(hashtab, chars("varX")).bigint,
        get_hashtab(hashtab, chars("varY")).bigint,
        get_hashtab(hashtab, chars("result")).bigint
    );
    free_hashtab(hashtab);
    return 0;
}

```
