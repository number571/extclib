#include <stdio.h>

#include "extclib/list.h"
#include "extclib/stack.h"

// gcc main.c extclib/extclib.o -o main

int main(void) {
    List *list = new_list(STACK_ELEM);

    push_list(list, stack(new_stack(10, DECIMAL_ELEM)));
    push_list(list, stack(new_stack(10, STRING_ELEM)));
    push_list(list, stack(new_stack(10, REAL_ELEM)));

    Stack *stack = get_list(list, 2).stack;
    push_stack(stack, real(5.71));
    push_stack(stack, real(1.23));

    stack = get_list(list, 0).stack;
    push_stack(stack, decimal(555));
    push_stack(stack, decimal(666));

    stack = get_list(list, 1).stack;
    push_stack(stack, string("hello"));
    push_stack(stack, string("world"));

    println_list(list);
    free_list(list);
    return 0;
}
