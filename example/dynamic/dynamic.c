#include "extclib/dynamic.h"

// gcc main.c extclib/dynamic.c extclib/type.c -o main

int main(void) {
    Dynamic *dynamic = new_dynamic(STRING_TYPE, string("hello, world!"));
    println_dynamic(dynamic);
    free_dynamic(dynamic);
    return 0;
}
