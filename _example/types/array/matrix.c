#include "extclib/types/array.h"

int main(void) {
    Array *array = new_array(3, ARRAY_TYPE);
    for (size_t i = 0; i < 3; ++i) {
        push_stack(array, new_array(3, DECIMAL_TYPE));
        for (size_t j = 0; j < 3; ++j) {
            push_stack(get_array(array, i).array, 0);
        }
    }
    for (size_t i = 0; i < 3; ++i) {
        println_array(get_array(array, i).array);
    }
    free_array(array);
    return 0;
}
