#include <stdio.h>

#include "extclib/list.h"

// gcc main.c extclib/list.c extclib/type.c -o main

int main(void) {
    List *list = new_list(STRING_TYPE);

    set_list(list, 0, string("A"));
    set_list(list, 0, string("B"));
    push_list(list, string("C"));

    printf("%d\n", size_list(list));

    int32_t index = 0;
    if ((index = in_list(list, string("B"))) != -1) {
        printf("%s\n", get_list(list, index).string);
        del_list(list, index);
    }

    printf("%s\n", pop_list(list).string);

    println_list(list);
    free_list(list);
    return 0;
}
