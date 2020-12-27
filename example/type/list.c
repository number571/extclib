#include "extclib/type.h"

#include <stdio.h>

int main(void) {
	int size, x;
	type_list *ls = type_list_new();

	x = 1; 
	for (int i = 0; i < 5; ++i) {
		type_list_insert(ls, i, &x, sizeof(x));
		++x; 
	}

	type_list_delete(ls, 0);
	type_list_delete(ls, 3);

	for (int i = 0; i < type_list_size(ls); ++i) {
		int *z = type_list_select(ls, i, &size);
		printf("%d\n", *z);
	}	

	type_list_free(ls);
	return 0;
}
