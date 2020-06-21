#include "extclib/types/dynamic.h"

int main(void) {
    Dynamic *dynamic = new_dynamic(STRING_TYPE, string("hello, world!"));
    println_dynamic(dynamic);
    free_dynamic(dynamic);
    return 0;
}
