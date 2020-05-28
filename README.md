# extclib
> Extended C library.

### Compile library:
```
$ make -C extclib/
$ cc main.c extclib/extclib.o -o main
```

### Used libraries:
1. bigdigits: [di-mgt.com.au/bigdigits.html](https://di-mgt.com.au/bigdigits.html);

### Example (IO, HashTab, BigInt):
```c
#include "extclib/io.h"
#include "extclib/hashtab.h"
#include "extclib/bigint.h"

// gcc main.c extclib/extclib.o -o main

int main(void) {
    HashTab *hashtab = new_hashtab(250, STRING_ELEM, BIGINT_ELEM);

    set_hashtab(hashtab, string("varX"), bigint(new_bigint("872138712637512787387124821738712648712736128749182")));
    set_hashtab(hashtab, string("varY"), bigint(new_bigint("675346126835124712346172467268375128731")));

    // result <- varX
    set_hashtab(hashtab, string("result"), bigint(new_bigint("")));
    cpy_bigint(
        get_hashtab(hashtab, string("result")).bigint, 
        get_hashtab(hashtab, string("varX")).bigint
    );

    // result <- result * varY
    mul_bigint(
        get_hashtab(hashtab, string("result")).bigint, 
        get_hashtab(hashtab, string("varY")).bigint
    );

    fmt_println("(%B * %B = %B)", 
        get_hashtab(hashtab, string("varX")).bigint,
        get_hashtab(hashtab, string("varY")).bigint,
        get_hashtab(hashtab, string("result")).bigint
    );
    free_hashtab(hashtab);
    return 0;
}
```
