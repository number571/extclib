#include <stdio.h>

#include "extclib/bigint.h"

// gcc main.c extclib/bigint.c extclib/bigint/bigd.c extclib/bigint/bigdigits.c -o main

int main(void) {
    BigInt *x = new_bigint("1");
    BigInt *y = new_bigint("2");

    shl_bigint(x, 200); // x = 1606938044258990275541962092341162602522202993782792835301376
    div_bigint(x, y);   // x = 803469022129495137770981046170581301261101496891396417650688
    
    add_bigint(y, x);   // y = 803469022129495137770981046170581301261101496891396417650690
    mul_bigint(x, y);   // x = 645562469521727147413979793000752968582426448207305878207666446073206164494485840619503679482636980061969575786022174720

    print_bigint(x);

    free_bigint(x);
    free_bigint(y);
    return 0;
}
