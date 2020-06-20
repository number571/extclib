#include <stdio.h>

#include "extclib/types/array.h"

int main(void) {
    Array *array = new_array(1000, CHARS_TYPE);

    set_stack(array, 10, 10, 30);

    push_stack(array, chars("A"));
    push_stack(array, chars("B"));

    set_array(array, 12, chars("D"));
    set_array(array, 50, chars("C"));

    push_stack(array, chars("E"));

    println_stack(array);
    println_array(array);

    printf("%s\n", pop_stack(array).chars);
    printf("%s\n", pop_stack(array).chars);

    println_stack(array);

    free_array(array);
    return 0;
}
