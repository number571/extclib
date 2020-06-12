#include <stdio.h>

#include "extclib/types.h"

int main(void) {
    HashTab *hashtab = new_hashtab(10000, CHARS_TYPE, DECIMAL_TYPE);

    set_hashtab(hashtab, chars("A"), decimal(10));
    set_hashtab(hashtab, chars("B"), decimal(20));
    set_hashtab(hashtab, chars("C"), decimal(30));

    del_hashtab(hashtab, chars("B"));

    uint8_t *elem = "A";
    if (in_hashtab(hashtab, chars(elem))) {
        printf("%d\n", get_hashtab(hashtab, chars(elem)).decimal);
    }

    println_hashtab(hashtab);
    free_hashtab(hashtab);
    return 0;
}
