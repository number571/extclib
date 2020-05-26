#pragma once

#include <stddef.h>

#include "tree.h"
#include "type.h"

typedef struct HashTab {
    struct {
        vtype_t key;
        vtype_t value;
    } type;
    size_t size;
    Tree **table;
} HashTab;

extern HashTab *new_hashtab(size_t size, vtype_t key, vtype_t value);
extern void free_hashtab(HashTab *hashtab);

extern value_t get_hashtab(HashTab *hashtab, void *key);
extern void set_hashtab(HashTab *hashtab, void *key, void *value);
extern void del_hashtab(HashTab *hashtab, void *key);
extern _Bool in_hashtab(HashTab *hashtab, void *key);

extern void print_hashtab(HashTab *hashtab);
