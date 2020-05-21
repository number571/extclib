# extclib
> Extended C library.

### Compile library:
```
$ cd extclib/
$ make compile
```

### Example (HashTab):
```c
#include <stdio.h>

#include "extclib/hashtab.h"

// gcc main.c extclib/hashtab.c extclib/type.c extclib/tree.c -o main

int main(void) {
    HashTab *hashtab = new_hashtab(10000, STRING_ELEM, DECIMAL_ELEM);

    set_hashtab(hashtab, string("A"), decimal(10));
    set_hashtab(hashtab, string("B"), decimal(20));
    set_hashtab(hashtab, string("C"), decimal(30));

    del_hashtab(hashtab, string("B"));

    uint8_t *elem = "A";
    if (in_hashtab(hashtab, string(elem))) {
        printf("%d\n", get_hashtab(hashtab, string(elem)).decimal);
    }

    print_hashtab(hashtab);
    free_hashtab(hashtab);
    return 0;
}
```
