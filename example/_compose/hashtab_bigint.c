#include <stdio.h>

#include "extclib/hashtab.h"
#include "extclib/bigint.h"

// gcc main.c extclib/extclib.o -o main

int main(void) {
    HashTab *hashtab = new_hashtab(250, STRING_ELEM, BIGINT_ELEM);

    set_hashtab(hashtab, string("A"), bigint(new_bigint("8721387126375127864217387124821738712648712736128749182")));
    set_hashtab(hashtab, string("B"), bigint(new_bigint("675346126835124712346172467268375128731")));

    mul_bigint(
    	get_hashtab(hashtab, string("A")).bigint, 
    	get_hashtab(hashtab, string("B")).bigint
    );
    println_bigint(get_hashtab(hashtab, string("A")).bigint);

    println_hashtab_format(hashtab);
    free_hashtab(hashtab);
    return 0;
}
