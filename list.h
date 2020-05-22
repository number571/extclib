#pragma once

#include <stddef.h>

#include "type.h"

typedef struct List {
    vtype_t type;
    size_t size;
    struct list_node *node;
} List;

extern List *new_list(vtype_t type);
extern void free_list(List *list);

extern void push_list(List *list, void *value);
extern value_t pop_list(List *list);

extern value_t get_list(List *list, size_t index);
extern int8_t set_list(List *list, size_t index, void *value);
extern int8_t del_list(List *list, size_t index);
extern int32_t in_list(List *list, void *value);

extern void print_list(List *list);
