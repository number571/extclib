#include <stdio.h>

#include "extclib/list.h"

// gcc main.c extclib/list.c extclib/type.c -o main

int main(void) {
    List *list = new_list(STRING_ELEM);

    set_list(list, 0, string("A"));
    set_list(list, 0, string("B"));
    push_list(list, string("C"));

    int32_t index = 0;
    if ((index = in_list(list, string("B"))) != -1) {
        del_list(list, index);
    }

    printf("%s\n", pop_list(list).string);

    print_list(list);
    free_list(list);
    return 0;
}
