#include "extclib/type.h"

#include <stdio.h>

int main(void) {
	int size, x;
	type_hashtab *ht = type_hashtab_new(250);

	x = 1; type_hashtab_insert(ht, "one", &x, sizeof(x));
	x = 2; type_hashtab_insert(ht, "two", &x, sizeof(x));
	x = 3; type_hashtab_insert(ht, "three", &x, sizeof(x));

	type_hashtab_delete(ht, "two");

	if (type_hashtab_select(ht, "two", &size) == NULL) {
		printf("deleted\n");
	}

	int *z = type_hashtab_select(ht, "three", &size);
	printf("num: %d, sizeof: %d\n", *z, size);

	type_hashtab_free(ht);
	return 0;
}
