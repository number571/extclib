#include "extclib/types.h"

int main(void) {
    HashTab *hashtab = new_hashtab(100, STRING_TYPE, DYNAMIC_TYPE);

    set_hashtab(hashtab, string("A"), new_dynamic(LIST_TYPE, new_list(DECIMAL_TYPE)));
    set_hashtab(hashtab, string("B"), new_dynamic(BIGINT_TYPE, new_bigint("87215378126371524781263781")));
    set_hashtab(hashtab, string("C"), new_dynamic(STRING_TYPE, string("hello, world!")));

    push_list(value_dynamic(get_hashtab(hashtab, string("A")).dynamic).list, decimal(5));
    push_list(value_dynamic(get_hashtab(hashtab, string("A")).dynamic).list, decimal(10));
    push_list(value_dynamic(get_hashtab(hashtab, string("A")).dynamic).list, decimal(20));

    println_dynamic(get_hashtab(hashtab, string("B")).dynamic);

    println_hashtab(hashtab);
    free_hashtab(hashtab);
    return 0;
}
