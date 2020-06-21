#include <stdio.h>

#include "extclib/types/hashtab.h"

int main(void) {
    HashTab *hashtab = new_hashtab(10000, STRING_TYPE, DECIMAL_TYPE);

    set_hashtab(hashtab, string("A"), decimal(10));
    set_hashtab(hashtab, string("B"), decimal(20));
    set_hashtab(hashtab, string("C"), decimal(30));

    del_hashtab(hashtab, string("B"));

    char *elem = "A";
    if (in_hashtab(hashtab, string(elem))) {
        printf("%d\n", get_hashtab(hashtab, string(elem)).decimal);
    }

    println_hashtab(hashtab);
    free_hashtab(hashtab);
    return 0;
}
