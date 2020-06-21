#include <stdio.h>

#include "extclib/types/array.h"

int main(void) {
    Array *array = new_array(1000, STRING_TYPE);

    set_stack(array, 10, 10, 30);

    push_stack(array, string("A"));
    push_stack(array, string("B"));

    set_array(array, 12, string("D"));
    set_array(array, 50, string("C"));

    push_stack(array, string("E"));

    println_stack(array);
    println_array(array);

    printf("%s\n", pop_stack(array).string);
    printf("%s\n", pop_stack(array).string);

    println_stack(array);

    free_array(array);
    return 0;
}
