#include <stdio.h>

#include "extclib/encoding.h"

char *json_str = "{"
"   \"str1\": \"qwe\","
"   \"num1\": 123,"
"   \"array\": [111.25, 222.50, 333.75],"
"   \"object\": {\"obj1\": \"objstr\", \"obj2\": 555}"
"}";

int main(void) {
    JSON *elem, *temp;
    JSON *json = decode_json(json_str);

    elem = get_json(json, "str1", STRING_TYPE);
    printf("%s\n", string_json(elem));

    elem = get_json(json, "num1", DECIMAL_TYPE);
    printf("%d\n", decimal_json(elem));

    JSON *array = get_json(json, "array", ARRAY_TYPE);

    for (size_t i = 0; i < arrsize_json(array); ++i) {
        elem = arritem_json(array, i, REAL_TYPE);
        printf("%f\n", real_json(elem));
    }

    elem = get_json(json, "object", DYNAMIC_TYPE);
    
    temp = get_json(elem, "obj1", STRING_TYPE);
    printf("%s\n", string_json(temp));

    temp = get_json(elem, "obj2", DECIMAL_TYPE);
    printf("%d\n", decimal_json(temp));

    free_json(json);
    return 0;
}
