#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "type.h"
#include "tree.h"
#include "list.h"
#include "hashtab.h"
#include "stack.h"
#include "bigint.h"

typedef struct tree_node {
    struct {
        value_t key;
        value_t value;
    } data;
    _Bool exist;
    struct tree_node *left;
    struct tree_node *right;
    struct tree_node *parent;
} tree_node;

typedef struct Tree {
    struct {
        vtype_t key;
        vtype_t value;
    } type;
    size_t size;
    struct tree_node *node;
} Tree;

static tree_node *_new_node(vtype_t tkey, vtype_t tvalue, void *key, void *value);
static void _set_tree(Tree *tree, tree_node *node, vtype_t tkey, vtype_t tvalue, void *key, void *value);
static void _set_key(tree_node *node, vtype_t tkey, void *key);
static void _set_value(tree_node *node, vtype_t tvalue, void *value);
static void _free_tree(Tree *tree, tree_node *node);
static void _free_node_tree(Tree *tree, tree_node *node);
static void _print_tree(tree_node *node, vtype_t tkey, vtype_t tvalue);
static void _print_tree_branches(tree_node *node, vtype_t tkey, vtype_t tvalue);
static void _print_tree_elem(tree_node *node, vtype_t tkey, vtype_t tvalue);
static tree_node *_get_tree(tree_node *node, vtype_t tkey, void *key);
static tree_node *_del1_tree(Tree *tree, vtype_t tkey, void *key);
static void _del2_tree(Tree *tree, tree_node *node);
static void _del3_tree(Tree *tree, tree_node *node);
static int8_t _cmp_tree(vtype_t tkey, vtype_t tvalue, tree_node *x, tree_node *y);

extern Tree *new_tree(vtype_t key, vtype_t value) {
    switch(key){
        case DECIMAL_ELEM: case STRING_ELEM:
            break;
        default:
            fprintf(stderr, "%s\n", "key type not supported");
            return NULL;
    }
    switch(value) {
        case DECIMAL_ELEM: 
        case REAL_ELEM: 
        case STRING_ELEM: 
        case LIST_ELEM: 
        case TREE_ELEM: 
        case HASHTAB_ELEM: 
        case STACK_ELEM:
        case BIGINT_ELEM:
            break;
        default:
            fprintf(stderr, "%s\n", "value type not supported");
            return NULL;
    }
    Tree *tree = (Tree*)malloc(sizeof(Tree));
    tree->type.key = key;
    tree->type.value = value;
    tree->node = NULL;
    tree->size = 0;
    return tree;
}

extern void free_tree(Tree *tree) {
    _free_tree(tree, tree->node);
    free(tree);
}

extern _Bool in_tree(Tree *tree, void *key) {
    return _get_tree(tree->node, tree->type.key, key) != NULL;
}

extern value_t get_tree(Tree *tree, void *key) {
    tree_node *node = _get_tree(tree->node, tree->type.key, key);
    if (node == NULL) {
        fprintf(stderr, "%s\n", "value undefined");
        value_t none;
        return none;
    }
    return node->data.value;
}

extern void set_tree(Tree *tree, void *key, void *value) {
    if (tree->node == NULL) {
        tree->node = _new_node(tree->type.key, tree->type.value, key, value);
        tree->size += 1;
        return;
    }
    _set_tree(tree, tree->node, tree->type.key, tree->type.value, key, value);
}

extern void del_tree(Tree *tree, void *key) {
    tree_node *node = _del1_tree(tree, tree->type.key, key);
    if (node == NULL) {
        return;
    }
    if (node->left != NULL && node->right != NULL) {
        _del3_tree(tree, node);
        return;
    }
    _del2_tree(tree, node);
    return;
}

extern int8_t cmp_tree(Tree *x, Tree *y) {
    if (x->type.key != y->type.key) {
        return -1;
    }
    if (x->type.value != y->type.value) {
        return -2;
    }
    if (x->size != y->size) {
        return 2;
    }
    return !_cmp_tree(x->type.key, x->type.value, x->node, y->node);
}

static int8_t _cmp_tree(vtype_t tkey, vtype_t tvalue, tree_node *x, tree_node *y) {
    if (x == NULL && y == NULL) {
        return 1;
    }
    if (x != NULL && y != NULL) {
        _Bool fkey = 0;
        _Bool fval = 0;
        switch(tkey) {
            case DECIMAL_ELEM:
                fkey = x->data.key.decimal == y->data.key.decimal;
            break;
            case STRING_ELEM:
                fkey = strcmp(x->data.key.string, y->data.key.string) == 0;
            break;
        }
        switch(tvalue) {
            case DECIMAL_ELEM:
                fval = x->data.value.decimal == y->data.value.decimal;
            break;
            case REAL_ELEM:
                fval = x->data.value.real == y->data.value.real;
            break;
            case STRING_ELEM:
                fval = strcmp(x->data.value.string, y->data.value.string) == 0;
            break;
            case LIST_ELEM:
                fval = cmp_list(x->data.value.list, y->data.value.list) == 0;
            break;
            case TREE_ELEM:
                fval = cmp_tree(x->data.value.tree, y->data.value.tree) == 0;
            break;
            case HASHTAB_ELEM:
                fval = cmp_hashtab(x->data.value.hashtab, y->data.value.hashtab) == 0;
            break;
            case STACK_ELEM:
                fval = cmp_stack(x->data.value.stack, y->data.value.stack) == 0;
            break;
            case BIGINT_ELEM:
                fval = cmp_bigint(x->data.value.bigint, y->data.value.bigint) == 0;
            break;
        }
        return fkey && fval && _cmp_tree(tkey, tvalue, x->left, y->left) && _cmp_tree(tkey, tvalue, x->right, y->right);
    }
    return 0;
}

extern size_t size_tree(Tree *tree) {
    return tree->size;
}

extern size_t sizeof_tree(void) {
    return sizeof(Tree);
}

extern void print_tree(Tree *tree) {
    printf("#T[ ");
    _print_tree(tree->node, tree->type.key, tree->type.value);
    putchar(']');
}

extern void println_tree(Tree *tree) {
    print_tree(tree);
    putchar('\n');
}

extern void print_tree_branches(Tree *tree) {
    _print_tree_branches(tree->node, tree->type.key, tree->type.value);
}

extern void println_tree_branches(Tree *tree) {
    _print_tree_branches(tree->node, tree->type.key, tree->type.value);
    putchar('\n');
}

static tree_node *_new_node(vtype_t tkey, vtype_t tvalue, void *key, void *value) {
    tree_node *node = (tree_node*)malloc(sizeof(tree_node));
    node->exist = 0;
    _set_key(node, tkey, key);
    _set_value(node, tvalue, value);
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

static void _set_key(tree_node *node, vtype_t tkey, void *key) {
    switch(tkey) {
        case DECIMAL_ELEM:
            node->data.key.decimal = (int32_t)(intptr_t)key;
        break;
        case STRING_ELEM:
            node->data.key.string = (uint8_t*)key;
        break;
    }
}

static void _set_value(tree_node *node, vtype_t tvalue, void *value) {
    switch(tvalue) {
        case DECIMAL_ELEM:
            node->data.value.decimal = (int32_t)(intptr_t)value;
        break;
        case REAL_ELEM:
            node->data.value.real = *(double*)value;
            free((double*)value);
        break;
        case STRING_ELEM:
            node->data.value.string = (uint8_t*)value;
        break;
        case LIST_ELEM:
            if (node->exist) {
                free_list(node->data.value.list);
            }
            node->data.value.list = (struct List*)value;
        break;
        case TREE_ELEM:
            if (node->exist) {
                free_tree(node->data.value.tree);
            }
            node->data.value.tree = (struct Tree*)value;
        break;
        case HASHTAB_ELEM:
            if (node->exist) {
                free_hashtab(node->data.value.hashtab);
            }
            node->data.value.hashtab = (struct HashTab*)value;
        break;
        case STACK_ELEM:
            if (node->exist) {
                free_stack(node->data.value.stack);
            }
            node->data.value.stack = (struct Stack*)value;
        break;
        case BIGINT_ELEM:
            if (node->exist) {
                free_bigint(node->data.value.bigint);
            }
            node->data.value.bigint = (struct BigInt*)value;
        break;
    }
    node->exist = 1;
}

static void _set_tree(Tree *tree, tree_node *node, vtype_t tkey, vtype_t tvalue, void *key, void *value) {
    int cond = 0;
    switch(tkey) {
        case DECIMAL_ELEM:
            if ((int32_t)(intptr_t)key > node->data.key.decimal) {
                if (node->right == NULL) {
                    node->right = _new_node(tkey, tvalue, key, value);
                    node->right->parent = node;
                    tree->size += 1;
                } else {
                    _set_tree(tree, node->right, tkey, tvalue, key, value);
                }
            } else if ((int32_t)(intptr_t)key < node->data.key.decimal) {
                if (node->left == NULL) {
                    node->left = _new_node(tkey, tvalue, key, value);
                    node->left->parent = node;
                    tree->size += 1;
                } else {
                    _set_tree(tree, node->left, tkey, tvalue, key, value);
                }
            } else {
                _set_value(node, tvalue, value);
            }
        break;
        case STRING_ELEM:
            cond = strcmp((uint8_t*)key, node->data.key.string);
            if (cond > 0) {
                if (node->right == NULL) {
                    node->right = _new_node(tkey, tvalue, key, value);
                    node->right->parent = node;
                    tree->size += 1;
                } else {
                    _set_tree(tree, node->right, tkey, tvalue, key, value);
                }
            } else if (cond < 0) {
                if (node->left == NULL) {
                    node->left = _new_node(tkey, tvalue, key, value);
                    node->left->parent = node;
                    tree->size += 1;
                } else {
                    _set_tree(tree, node->left, tkey, tvalue, key, value);
                }
            } else {
                _set_value(node, tvalue, value);
            }
        break;
    }
}

static tree_node *_get_tree(tree_node *node, vtype_t tkey, void *key) {
    int cond = 0;
    if (node == NULL) {
        return NULL;
    }
    switch(tkey) {
        case DECIMAL_ELEM:
            if ((int32_t)(intptr_t)key > node->data.key.decimal) {
                return _get_tree(node->right, tkey, key);
            } else if ((int32_t)(intptr_t)key < node->data.key.decimal) {
                return _get_tree(node->left, tkey, key);
            }
        break;
        case STRING_ELEM:
            cond = strcmp((uint8_t*)key, node->data.key.string);
            if (cond > 0) {
                return _get_tree(node->right, tkey, key);
            } else if (cond < 0) {
                return _get_tree(node->left, tkey, key);
            }
        break;
    }
    return node;
}

static tree_node *_del1_tree(Tree *tree, vtype_t tkey, void *key) {
    tree_node *node = tree->node;
    node =  _get_tree(node, tkey, key);
    if (node == NULL) {
        return NULL;
    }
    if (node->left != NULL || node->right != NULL) {
        return node;
    }
    tree_node *parent = node->parent;
    if (parent == NULL) {
        tree->node = NULL;
    } else if (parent->left == node) {
        parent->left = NULL;
    } else {
        parent->right = NULL;
    }
    tree->size -= 1;
    _free_node_tree(tree, node);
    free(node);
    return NULL;
}

static void _del2_tree(Tree *tree, tree_node *node) {
    tree_node *parent = node->parent;
    tree_node *temp;
    if (node->right != NULL) {
        temp = node->right;
    } else {
        temp = node->left;
    }
    if (parent == NULL) {
        tree->node = temp;
    } else if (parent->left == node) {
        parent->left = temp;
    } else {
        parent->right = temp;
    }
    tree->size -= 1;
    temp->parent = parent;
    _free_node_tree(tree, node);
    free(node);
}

static void _del3_tree(Tree *tree, tree_node *node) {
    tree_node *ptr = node->right;
    while (ptr->left != NULL) {
        ptr = ptr->left;
    }
    node->data.key = ptr->data.key;
    node->data.value = ptr->data.value;
    tree_node *parent = ptr->parent;
    if (parent->left == ptr) {
        parent->left = NULL;
    } else {
        parent->right = NULL;
    }
    tree->size -= 1;
    _free_node_tree(tree, ptr);
    free(ptr);
}

static void _print_tree_elem(tree_node *node, vtype_t tkey, vtype_t tvalue) {
    switch(tkey) {
        case DECIMAL_ELEM:
            printf("{ %d => ", node->data.key.decimal);
        break;
        case STRING_ELEM:
            printf("{ '%s' => ", node->data.key.string);
        break;
    }
    switch(tvalue) {
        case DECIMAL_ELEM:
            printf("%d", node->data.value.decimal);
        break;
        case REAL_ELEM:
            printf("%lf", node->data.value.real);
        break;
        case STRING_ELEM:
            printf("'%s'", node->data.value.string);
        break;
        case LIST_ELEM: 
            print_list(node->data.value.list);
        break;
        case TREE_ELEM: 
            print_tree(node->data.value.tree);
        break;
        case HASHTAB_ELEM: 
            print_hashtab(node->data.value.hashtab);
        break;
        case STACK_ELEM:
            print_stack(node->data.value.stack);
        break;
        case BIGINT_ELEM:
            print_bigint(node->data.value.bigint);
        break;
    }
    printf(" } ");
}

static void _print_tree_branches(tree_node *node, vtype_t tkey, vtype_t tvalue) {
    if (node == NULL) {
        printf("null");
        return;
    }
    putchar('(');
    _print_tree_branches(node->left, tkey, tvalue);
    putchar(' ');
    _print_tree_elem(node, tkey, tvalue);
    _print_tree_branches(node->right, tkey, tvalue);
    putchar(')');
}

static void _print_tree(tree_node *node, vtype_t tkey, vtype_t tvalue) {
    if (node == NULL) {
        return;
    }
    _print_tree(node->left, tkey, tvalue);
    _print_tree_elem(node, tkey, tvalue);
    _print_tree(node->right, tkey, tvalue);
}

static void _free_tree(Tree *tree, tree_node *node) {
    if (node == NULL) {
        return;
    }
    _free_tree(tree, node->left);
    _free_tree(tree, node->right);
    _free_node_tree(tree, node);
    free(node);
}

static void _free_node_tree(Tree *tree, tree_node *node) {
    switch(tree->type.value) {
        case LIST_ELEM:
            free_list(node->data.value.list);
        break;
        case TREE_ELEM:
            free_tree(node->data.value.tree);
        break;
        case HASHTAB_ELEM:
            free_hashtab(node->data.value.hashtab);
        break;
        case STACK_ELEM: 
            free_stack(node->data.value.stack);
        break;
        case BIGINT_ELEM:
            free_bigint(node->data.value.bigint);
        break;
    }
}
