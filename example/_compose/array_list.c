#include "extclib/array.h"
#include "extclib/list.h"

int main(void) {
    Array *array = new_array(3, LIST_TYPE);

    push_stack(array, list(new_list(DECIMAL_TYPE)));
    push_stack(array, list(new_list(CHARS_TYPE)));
    push_stack(array, list(new_list(REAL_TYPE)));

    List *list = get_array(array, 2).list;
    push_list(list, real(5.71));
    push_list(list, real(1.23));

    list = get_array(array, 0).list;
    push_list(list, decimal(555));
    push_list(list, decimal(666));

    list = get_array(array, 1).list;
    push_list(list, chars("hello"));
    push_list(list, chars("world"));

    println_stack(array);
    free_array(array);
    return 0;
}
