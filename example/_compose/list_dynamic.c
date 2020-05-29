#include <stdio.h>

#include "extclib/list.h"
#include "extclib/dynamic.h"

// gcc main.c extclib/extclib.o -o main

int main(void) {
    List *list = new_list(DYNAMIC_ELEM);

    push_list(list, dynamic(new_dynamic('s', string("A"))));
    push_list(list, dynamic(new_dynamic('r', real(5.71))));
    push_list(list, dynamic(new_dynamic('d', decimal(333))));

    printf("%d\n", size_list(list));
    println_list(list);

    Dynamic *dynamic = get_list(list, 2).dynamic;
    printf("%d\n", type_dynamic(dynamic)); // DECIMAL_ELEM
    println_dynamic(dynamic); // -> printf("%d\n", decimal)
    
    free_list(list);
    return 0;
}
