#include <stdio.h>

#include "extclib/types.h"

int main(void) {
    List *list = new_list(DYNAMIC_TYPE);

    push_list(list, new_dynamic(STRING_TYPE, string("A")));
    push_list(list, new_dynamic(REAL_TYPE, real(5.71)));
    push_list(list, new_dynamic(DECIMAL_TYPE, decimal(333)));

    printf("%lu\n", size_list(list));
    println_list(list);

    Dynamic *dynamic = get_list(list, 2).dynamic;
    printf("%d\n", type_dynamic(dynamic)); // DECIMAL_TYPE
    println_dynamic(dynamic); // -> printf("%d\n", decimal)
    
    free_list(list);
    return 0;
}
