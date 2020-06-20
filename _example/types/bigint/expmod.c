#include "extclib/io.h"
#include "extclib/types/bigint.h"

int main(void) {
    BigInt *r = new_bigint("0");
    BigInt *x = new_bigint("8921739812739812738971298312");
    BigInt *e = new_bigint("9872139812748281361984912863981274981273162498171");
    BigInt *m = new_bigint("76362487298361248712874612987391624901242398462873479823642897316");
    
    expmod_bigint(r, x, e, m);
    printf_io("%B ^ %B = %B (mod %B)\n", x, e, r, m);

    free_bigint(x);
    free_bigint(e);
    free_bigint(m);
    free_bigint(r);
    return 0;
}
