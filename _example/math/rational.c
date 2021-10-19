#include <stdio.h>

#include "extclib/math/rational.h"

int main(void) {
    rational_t *x = rational_new(bigint_new("2"), bigint_new("3"));
    rational_t *y = rational_new(bigint_new("5"), bigint_new("4"));

    rational_add(x, x, y);
    rational_out(x, stdout);
    putchar('\n');

    rational_free(x);
    rational_free(y);

    return 0;
}
