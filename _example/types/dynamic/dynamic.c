#include "extclib/types/dynamic.h"

int main(void) {
    Dynamic *dynamic = new_dynamic(CHARS_TYPE, chars("hello, world!"));
    println_dynamic(dynamic);
    free_dynamic(dynamic);
    return 0;
}
