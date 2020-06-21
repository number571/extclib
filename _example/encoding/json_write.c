#include <stdio.h>
#include <stdlib.h>

#include "extclib/encoding.h"

int main(void) {
    JSON *json = new_json();

    set_json(json, "str1", string("qwe"), STRING_TYPE);
    set_json(json, "num1", decimal(123), DECIMAL_TYPE);

    set_json(json, "array", NULL, ARRAY_TYPE);

    JSON *arr = get_json(json, "array", ARRAY_TYPE);

    double nums[3] = {111.25, 222.50, 333.75};
    for (size_t i = 0; i < 3; ++i) {
        arrpush_json(arr, real(nums[i]), REAL_TYPE);
    }

    JSON *elem = new_json();
    set_json(json, "object", elem, DYNAMIC_TYPE);

    set_json(elem, "obj1", string("objstr"), STRING_TYPE);
    set_json(elem, "obj2", decimal(555), DECIMAL_TYPE);

    char *encoded = encode_json(json);
    printf("%s\n", encoded);

    free(encoded);
    free_json(json);
    return 0;
}
