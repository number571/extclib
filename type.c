#include "type.h"

#include <stdlib.h>
#include <string.h>

/* ------------- */
/* TYPE: HASHTAB */
/* ------------- */

typedef struct type_hashtab {
	int size;
	type_list **table;
} type_hashtab;

static unsigned int strhash(char *str, size_t size);

extern type_hashtab *type_hashtab_new(int size) {
	type_hashtab *ht = (type_hashtab*)malloc(sizeof(type_hashtab));
	ht->size = size;
	ht->table = (type_list**)malloc(sizeof(type_list*)*size);
	for (int i = 0; i < size; ++i) {
		ht->table[i] = type_list_new();
	}
	return ht;
}

extern void type_hashtab_free(type_hashtab *ht) {
	for (int i = 0; i < ht->size; ++i) {
		free(ht->table[i]);
	}
	free(ht->table);
	free(ht);
}

extern int type_hashtab_size(type_hashtab *ht) {
	return ht->size;
}

extern void *type_hashtab_select(type_hashtab *ht, char *key, int *size) {
	unsigned int hash = strhash(key, ht->size);
	size_t lenkey = strlen(key)+1;
	char *sel_elem;
	int sel_size;
	int i = 0;
	while ((sel_elem = type_list_select(ht->table[hash], i, &sel_size)) != NULL) {
		if (strcmp(sel_elem, key) == 0) {
			*size = sel_size - lenkey;
			return (void*)sel_elem + lenkey;
		}
		++i;
	}
	return NULL;
}

extern int type_hashtab_insert(type_hashtab *ht, char *key, void *elem, int size) {
	unsigned int hash = strhash(key, ht->size);
	size_t lenkey = strlen(key)+1;
	char *sel_elem;
	int sel_size, rc;
	int i = 0;
	char *val;
	while ((sel_elem = type_list_select(ht->table[hash], i, &sel_size)) != NULL) {
		if (strcmp(sel_elem, key) == 0) {
			break;
		}
		++i;
	}
	val = (char*)malloc(size+lenkey);
	memcpy(val, key, lenkey);
	memcpy(val+lenkey, elem, size);
	rc = type_list_insert(ht->table[hash], i, val, size+lenkey);
	free(val);
	return rc;
}

extern int type_hashtab_delete(type_hashtab *ht, char *key) {
	unsigned int hash = strhash(key, ht->size);
	char *sel_elem;
	int sel_size;
	int i = 0;
	while ((sel_elem = type_list_select(ht->table[hash], i, &sel_size)) != NULL) {
		if (strcmp(sel_elem, key) == 0) {
			return type_list_delete(ht->table[hash], i);
		}
		++i;
	}
	return -1;
}

static unsigned int strhash(char *str, size_t size) {
    unsigned int hashval;
    for (hashval = 0; *str != '\0'; ++str) {
        hashval = *str + 31 * hashval;
    }
    return hashval % size;
}

/* ---------- */
/* TYPE: LIST */
/* ---------- */

typedef struct type_list {
	int size;
	void *elem;
	struct type_list *next;
} type_list;

extern type_list *type_list_new(void) {
	type_list *ls = (type_list*)malloc(sizeof(type_list));
	ls->size = -1;
	ls->elem = NULL;
	ls->next = NULL;
	return ls;
}

extern void type_list_free(type_list *ls) {
	type_list *temp;
	while(ls != NULL) {
		temp = ls->next;
		free(ls->elem);
		free(ls);
		ls = temp;
	}
}

extern int type_list_find(type_list *ls, void *elem, int size) {
	int i = 0;
	while(ls->next != NULL) {
		ls = ls->next;
		if (ls->size == size && memcmp(ls->elem, elem, size) == 0) {
			return i;
		}
		++i;
	}
	return -1;
}

extern void *type_list_select(type_list *ls, int index, int *size) {
	int i = 0;
	while(ls->next != NULL && i < index) {
		ls = ls->next;
		++i;
	}
	if (ls->next == NULL) {
		return NULL;
	}
	*size = ls->next->size;
	return ls->next->elem;
}

extern int type_list_insert(type_list *ls, int index, void *elem, int size) {
	if (size <= 0) {
		return 1;
	}
	int i = 0;
	while(ls != NULL && i < index) {
		ls = ls->next;
		++i;
	}
	if (ls == NULL) {
		return 2;
	}
	if (ls->next == NULL) {
		ls->next = type_list_new();
		ls->next->elem = (void*)malloc(size);
		memcpy(ls->next->elem, elem, size);
	} else {
		ls->next->elem = (void*)realloc(ls->next->elem, size);
	}
	ls->next->size = size;
	memcpy(ls->next->elem, elem, size);
	return 0;
}

extern int type_list_delete(type_list *ls, int index) {
	int i = 0;
	type_list *temp;
	while(ls->next != NULL && i < index) {
		ls = ls->next;
		++i;
	}
	if (ls->next == NULL) {
		return 1;
	}
	temp = ls->next;
	ls->next = ls->next->next;
	free(temp);
	return 0;
}

extern int type_list_size(type_list *ls) {
	int i = 0;
	while(ls->next != NULL) {
		ls = ls->next;
		++i;
	}
	return i;
}

/* ----------- */
/* TYPE: STACK */
/* ----------- */

typedef struct type_stack {
	int size;
	int valsize;
	int currpos;
	char *buffer;
} type_stack;

extern type_stack *type_stack_new(int size, int valsize) {
	type_stack *st = (type_stack*)malloc(sizeof(type_stack));
	st->size = size;
	st->valsize = valsize;
	st->currpos = 0;
	st->buffer = (char*)malloc(size*valsize);
	return st;
}

extern void type_stack_free(type_stack *st) {
	free(st->buffer);
	free(st);
}

extern int type_stack_size(type_stack *st) {
	return st->currpos;
}

extern int type_stack_push(type_stack *st, void *elem) {
	if (st->currpos == st->size) {
		return 1;
	}
	memcpy(st->buffer + st->currpos * st->valsize, elem, st->valsize);
	st->currpos += 1;
	return 0;
}

extern void *type_stack_pop(type_stack *st) {
	if (st->currpos == 0) {
		return NULL;
	}
	st->currpos -= 1;
	return st->buffer + st->currpos * st->valsize;
}

extern int type_stack_set(type_stack *st, int index, void *elem) {
	if (index < 0 || index >= st->size) {
		return 1;
	}
	memcpy(st->buffer + index * st->valsize, elem, st->valsize);
	return 0;
}

extern void *type_stack_get(type_stack *st, int index) {
	if (index < 0 || index >= st->size) {
		return NULL;
	}
	return st->buffer + index * st->valsize;
}
