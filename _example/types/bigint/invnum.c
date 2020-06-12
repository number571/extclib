#include "extclib/io.h"
#include "extclib/bigint.h"

int main(void) {
    BigInt *x = new_bigint("8921739812739812738971298312");
    BigInt *y = new_bigint("9872139812748281361984912863981274981273162498171");
    BigInt *z = new_bigint("");
    inv_bigint(z, x, y);

    BigInt *t = load_bigint(x);
    mul_bigint(t, x, z);
    mod_bigint(t, t, y);

    printf_io("%B * %B = %B (mod %B)\n", x, z, t, y);

    free_bigint(x);
    free_bigint(y);
    free_bigint(z);
    free_bigint(t);
    return 0;
}
