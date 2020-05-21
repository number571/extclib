#pragma once

#include "type.h"

typedef struct Tree {
    struct {
        vtype_t key;
        vtype_t value;
    } type;
    struct tree_node *node;
} Tree;

extern Tree *new_tree(vtype_t key, vtype_t value);
extern void free_tree(Tree *tree);

extern value_t get_tree(Tree *tree, void *key);
extern void set_tree(Tree *tree, void *key, void *value);
extern void del_tree(Tree *tree, void *key);
extern _Bool in_tree(Tree *tree, void *key);

extern void print_tree(Tree *tree);
extern void print_tree_as_list(Tree *tree);
