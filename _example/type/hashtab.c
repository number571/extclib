#include "extclib/type/hashtab.h"

#include <stdio.h>

int main(void) {
	char *arr[4] = { "null", "one", "two", "three" };
	hashtab_t *ht = hashtab_new(250);

	for (int i = 0; i < 4; ++i) {
		hashtab_insert(ht, arr[i], &i, sizeof(i));
		printf("num: %d\n", *(int*)hashtab_select(ht, arr[i]));
	}

	hashtab_delete(ht, "two");
	if (hashtab_select(ht, "two") == NULL) {
		printf("deleted\n");
	}

	hashtab_free(ht);
	return 0;
}
