#include "extclib/type/list.h"

#include <stdio.h>

int main(void) {
	list_t *ls = list_new();
	for (int i = 0, x = 1; i < 5; ++i, x <<= 1) {
		list_insert(ls, i, &x, sizeof(x));
	}

	list_delete(ls, 0);
	list_delete(ls, 1);

	for (int i = 0; i < list_size(ls); ++i) {
		printf("%d\n", *(int*)list_select(ls, i));
	}	

	list_free(ls);
	return 0;
}
