#include "extclib/types.h"

int main(void) {
    HashTab *hashtab = new_hashtab(100, CHARS_TYPE, DYNAMIC_TYPE);

    set_hashtab(hashtab, chars("A"), dynamic(new_dynamic(LIST_TYPE, list(new_list(DECIMAL_TYPE)))));
    set_hashtab(hashtab, chars("B"), dynamic(new_dynamic(BIGINT_TYPE, bigint(new_bigint("87215378126371524781263781")))));
    set_hashtab(hashtab, chars("C"), dynamic(new_dynamic(CHARS_TYPE, chars("hello, world!"))));

    push_list(value_dynamic(get_hashtab(hashtab, chars("A")).dynamic).list, decimal(5));
    push_list(value_dynamic(get_hashtab(hashtab, chars("A")).dynamic).list, decimal(10));
    push_list(value_dynamic(get_hashtab(hashtab, chars("A")).dynamic).list, decimal(20));

    println_dynamic(get_hashtab(hashtab, chars("B")).dynamic);

    println_hashtab(hashtab);
    free_hashtab(hashtab);
    return 0;
}
