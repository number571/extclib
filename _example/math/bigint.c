#include <stdio.h>

#include "extclib/math/bigint.h"

int main(void) {
	bigint_t *x = bigint_new("892734892374827498236498237498274982749285");
	bigint_t *y = bigint_new("1239812903148271938174612938719824611893712846219");

	bigint_mul(x, x, y);
	bigint_out(x, stdout, 10);
	printf("\n");

	bigint_free(x);
	bigint_free(y);
	return 0;
}
