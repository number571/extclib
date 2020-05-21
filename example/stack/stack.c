#include <stdio.h>

#include "extclib/stack.h"

// gcc main.c extclib/stack.c extclib/type.c -o main

int main(void) {
    Stack *stack = new_stack(1000, STRING_ELEM);

    push_stack(stack, string("A"));
    push_stack(stack, string("B"));
    push_stack(stack, string("C"));

    print_stack(stack);

    printf("%s\n", pop_stack(stack).string);
    printf("%s\n", pop_stack(stack).string);
    printf("%s\n", pop_stack(stack).string);

    free_stack(stack);
    return 0;
}
