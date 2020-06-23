# extclib
> Extended C library.

### Implemented:
1. Types: Array, BigInt, Dynamic, HashTab, List, Tree;
2. Crypto: AES[ECB, CBC], RSA[OAEP, SIGN], SHA256, HMAC[SHA256], RAND;
3. Encoding: Base64, JSON;
4. Database: SQLite;
5. Net: Linux[arpa/inet.h], Windows[winsock2.h];

### Used libraries:
1. gmp: [gmplib.org](https://gmplib.org/);
2. openssl: [openssl.org](https://www.openssl.org/);
3. cJSON: [cJSON.git](https://github.com/DaveGamble/cJSON.git/);
4. sqlite3: [sqlite.org](https://www.sqlite.org/);

### Compile library:
```
/* install 'gmp', 'openssl', 'cJSON', 'sqlite3' libraries before compilation */
$ make -C extclib/

/* linux compile: */
$ cc main.c extclib/extclib.o -o main -lgmp -lcrypto -lcjson

/* windows compile: */
$ cc main.c extclib/extclib.o -o main -lgmp -lcrypto -lcjson -lws2_32
```

### Usage restrictions:
1. Recursive hash tables, trees, lists, arrays are not allowed. Cross-recursive is dangerous;
2. Values (and/or keys) in hash tables, trees, lists, arrays are freed up automatically when deleting or replacing an object's value;

### Example (IO, Types):
```c
#include "extclib/io.h"
#include "extclib/types.h"

int main(void) {
    HashTab *hashtab = new_hashtab(250, STRING_TYPE, BIGINT_TYPE);

    set_hashtab(hashtab, string("varX"), new_bigint("872138712637512787387124821738712648712736128749182"));
    set_hashtab(hashtab, string("varY"), new_bigint("675346126835124712346172467268375128731"));
    set_hashtab(hashtab, string("result"), new_bigint(""));

    // result <- varX * varY
    mul_bigint(
        get_hashtab(hashtab, string("result")).bigint, 
        get_hashtab(hashtab, string("varX")).bigint,
        get_hashtab(hashtab, string("varY")).bigint
    );

    printf_io("(%B * %B = %B)\n", 
        get_hashtab(hashtab, string("varX")).bigint,
        get_hashtab(hashtab, string("varY")).bigint,
        get_hashtab(hashtab, string("result")).bigint
    );
    free_hashtab(hashtab);
    return 0;
}

```
