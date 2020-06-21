#include <stdio.h>

#include "extclib/types.h"

int main(void) {
    HashTab *hashtab = new_hashtab(10000, BIGINT_TYPE, DECIMAL_TYPE);
    BigInt *elem = new_bigint("");

    set_hashtab(hashtab, new_bigint("9327139817249812739812649821"), decimal(10));
    set_hashtab(hashtab, new_bigint("513425353464565475676787978978675"), decimal(20));
    set_hashtab(hashtab, new_bigint("1092386412934213087123912498173"), decimal(30));

    cpystr_bigint(elem, "513425353464565475676787978978675");
    del_hashtab(hashtab, elem);

    cpystr_bigint(elem, "9327139817249812739812649821");
    if (in_hashtab(hashtab, elem)) {
        printf("%d\n", get_hashtab(hashtab, elem).decimal);
    }

    println_hashtab(hashtab);

    free_bigint(elem);
    free_hashtab(hashtab);
    return 0;
}
