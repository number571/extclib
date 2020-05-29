#include <stdio.h>

#include "extclib/io.h"
#include "extclib/bigint.h"

int main(void) {
    BigInt *bigint = new_bigint("92179841239812749213981274912731");
    printf_io("(%B)\n", bigint);
    free_bigint(bigint);
    return 0;
}
