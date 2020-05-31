#include <stdio.h>

#include "extclib/hashtab.h"
#include "extclib/string.h"

int main(void) {
    HashTab *hashtab = new_hashtab(10000, STRING_TYPE, DECIMAL_TYPE);
    String *elem = new_string("");

    set_hashtab(hashtab, string(new_string("A")), decimal(10));
    set_hashtab(hashtab, string(new_string("B")), decimal(20));
    set_hashtab(hashtab, string(new_string("C")), decimal(30));

    cpy_in_string(elem, "C");
    del_hashtab(hashtab, string(elem));

    cpy_in_string(elem, "A");
    if (in_hashtab(hashtab, string(elem))) {
        printf("%d\n", get_hashtab(hashtab, string(elem)).decimal);
    }

    println_hashtab(hashtab);

    free_string(elem);
    free_hashtab(hashtab);
    return 0;
}
