#include <stdio.h>

#include "extclib/types/list.h"

int main(void) {
    List *list = new_list(CHARS_TYPE);

    set_list(list, 0, chars("A"));
    set_list(list, 0, chars("B"));
    push_list(list, chars("C"));

    printf("%ld\n", size_list(list));

    int32_t index = 0;
    if ((index = in_list(list, chars("B"))) != -1) {
        printf("%s\n", get_list(list, index).chars);
        del_list(list, index);
    }

    printf("%s\n", pop_list(list).chars);

    println_list(list);
    free_list(list);
    return 0;
}
