#include <stdio.h>

#include "extclib/string.h"

int main(void) {
    String *string = new_string("hello");
    cat_in_string(string, ", world!");
    println_string(string);
    free_string(string);
    return 0;
}
