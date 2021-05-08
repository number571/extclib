#include "extclib/type/stack.h"

#include <stdio.h>

int main(void) {
	stack_t *st = stack_new(100, sizeof(int*));
	
	for (int i = 0; i < 10; ++i) {
		stack_push(st, &i);
	}

	for (int i = 0; i < 10; ++i) {
		printf("%d\n", *(int*)stack_pop(st));
	}

	stack_set(st, 5, (int[]){12345});
	printf("[%d]\n", *(int*)stack_get(st, 5));

	stack_free(st);
	return 0;
}
