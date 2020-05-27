#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "type.h"
#include "list.h"
#include "tree.h"
#include "hashtab.h"
#include "stack.h"
#include "bigint.h"

typedef struct list_node {
    value_t value;
    struct list_node *next;
} list_node;

typedef struct List {
    vtype_t type;
    size_t size;
    struct list_node *node;
} List;

static list_node *_new_node(vtype_t type, void *value);
static void _print_list(vtype_t type, list_node *node);
static void _free_list(List *list, list_node *node);

extern List *new_list(vtype_t type) {
    switch(type) {
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
    List *list = (List*)malloc(sizeof(List));
    list->type = type;
    list->size = 0;
    list->node = NULL;
    return list;
}

extern int32_t in_list(List *list, void *value) {
    int32_t index = 0;
    list_node *node = list->node;
    while(node != NULL) {
        _Bool flag = 0;
        switch(list->type) {
            case DECIMAL_ELEM:
                flag = (int32_t)(intptr_t)value == node->value.decimal;
            break;
            case REAL_ELEM:
                flag = *(double*)value == node->value.real;
                if(flag) {
                    free((double*)value);
                }
            break;
            case STRING_ELEM:
                flag = strcmp((uint8_t*)value, node->value.string) == 0;
            break;
            case LIST_ELEM:
                flag = cmp_list((List*)value, node->value.list) == 0;
            break;
            case TREE_ELEM:
                flag = cmp_tree((Tree*)value, node->value.tree) == 0;
            break;
            case HASHTAB_ELEM:
                flag = cmp_hashtab((HashTab*)value, node->value.hashtab) == 0;
            break;
            case STACK_ELEM:
                flag = cmp_stack((Stack*)value, node->value.stack) == 0;
            break;
            case BIGINT_ELEM:
                flag = cmp_bigint(value, node->value.bigint) == 0;
            break;
        }
        if (flag) {
            return index;
        }
        node = node->next;
        ++index;
    }
    if(list->type == REAL_ELEM) {
        free((double*)value);
    }
    return -1;
}

extern int8_t cmp_list(List *x, List *y) {
    if (x->type != y->type) {
        return -1;
    }
    if (x->size != y->size) {
        return 2;
    }
    list_node *ptrx = x->node;
    list_node *ptry = y->node;
    while(ptrx != NULL) {
        _Bool fval = 0;
        switch(x->type) {
            case DECIMAL_ELEM:
                fval = ptrx->value.decimal == ptry->value.decimal;
            break;
            case REAL_ELEM:
                fval = ptrx->value.real == ptry->value.real;
            break;
            case STRING_ELEM:
                fval = strcmp(ptrx->value.string, ptry->value.string) == 0;
            break;
            case LIST_ELEM:
                fval = cmp_list(ptrx->value.list, ptry->value.list) == 0;
            break;
            case TREE_ELEM:
                fval = cmp_tree(ptrx->value.tree, ptry->value.tree) == 0;
            break;
            case HASHTAB_ELEM:
                fval = cmp_hashtab(ptrx->value.hashtab, ptry->value.hashtab) == 0;
            break;
            case STACK_ELEM:
                fval = cmp_stack(ptrx->value.stack, ptrx->value.stack) == 0;
            break;
            case BIGINT_ELEM:
                fval = cmp_bigint(ptrx->value.bigint, ptrx->value.bigint) == 0;
            break;
        }
        if (!fval) {
            return 1;
        }
        ptrx = ptrx->next;
        ptry = ptry->next;
    }
    return 0;
}

extern size_t size_list(List *list) {
    return list->size;
}

extern size_t sizeof_list(void) {
    return sizeof(List);
}

extern value_t get_list(List *list, size_t index) {
    list_node *node = list->node;
    while(node != NULL && index) {
        node = node->next;
        --index;
    }
    if(index != 0) {
        fprintf(stderr, "%s\n", "index > size");
        value_t none;
        return none;
    }
    if(node == NULL) {
        fprintf(stderr, "%s\n", "undefined");
        value_t none;
        return none;
    }
    return node->value;
}

extern int8_t set_list(List *list, size_t index, void *value) {
    list_node *prev_node = NULL;
    list_node *curr_node = list->node;
    while(curr_node != NULL && index) {
        prev_node = curr_node;
        curr_node = curr_node->next;
        --index;
    }
    if(index != 0) {
        return 1;
    }
    if(prev_node == NULL) {
        list_node *temp = list->node;
        list->node = _new_node(list->type, value);
        list->node->next = temp;
        list->size += 1;
        return 0;
    }
    list_node *new_node = _new_node(list->type, value);
    prev_node->next = new_node;
    new_node->next = curr_node;
    list->size += 1;
    return 0;
}

extern int8_t del_list(List *list, size_t index) {
    list_node *prev_node = NULL;
    list_node *curr_node = list->node;
    while(curr_node != NULL && index) {
        prev_node = curr_node;
        curr_node = curr_node->next;
        --index;
    }
    if(index != 0) {
        return 1;
    }
    if(curr_node == NULL) {
        return 2;
    }
    list_node *temp = curr_node->next;
    if(prev_node == NULL) {
        list->node = temp;
    } else {
        prev_node->next = temp;
    }
    free(curr_node);
    list->size -= 1;
    return 0;
}

extern void push_list(List *list, void *value) {
    if(list->node == NULL) {
        list->node = _new_node(list->type, value);
        list->size += 1;
        return;
    }
    list_node *node = list->node;
    while(node->next != NULL) {
        node = node->next;
    }
    node->next = _new_node(list->type, value);
    list->size += 1;
    return;
}

extern value_t pop_list(List *list) {
    list_node *curr_node = list->node;
    if(curr_node == NULL) {
        fprintf(stderr, "%s\n", "stack overflow");
        value_t none;
        return none;
    }
    if(curr_node->next == NULL) {
        list->node = NULL;
        value_t value = curr_node->value;
        free(curr_node);
        list->size -= 1;
        return value;
    }
    list_node *prev_node = curr_node;
    curr_node = curr_node->next;
    while(curr_node->next != NULL) {
        prev_node = curr_node;
        curr_node = curr_node->next;
    }
    prev_node->next = NULL;
    value_t value = curr_node->value;
    free(curr_node);
    list->size -= 1;
    return value;
}

extern void print_list(List *list) {
    _print_list(list->type, list->node);
}

extern void println_list(List *list) {
    _print_list(list->type, list->node);
    putchar('\n');
}

extern void free_list(List *list) {
    list_node *temp = NULL;
    list_node *node = list->node;
    while(node != NULL) {
        temp = node->next;
        _free_list(list, node);
        free(node);
        node = temp;
    }
    free(list);
}

static void _free_list(List *list, list_node *node) {
    switch(list->type) {
        case LIST_ELEM:
            free_list(node->value.list);
        break;
        case TREE_ELEM:
            free_tree(node->value.tree);
        break;
        case HASHTAB_ELEM:
            free_hashtab(node->value.hashtab);
        break;
        case STACK_ELEM: 
            free_stack(node->value.stack);
        break;
        case BIGINT_ELEM:
            free_bigint(node->value.bigint);
        break;
    }
}

static list_node *_new_node(vtype_t type, void *value) {
    list_node *node = (list_node*)malloc(sizeof(list_node));
    node->next = NULL;
    switch(type) {
        case DECIMAL_ELEM:
            node->value.decimal = (int32_t)(intptr_t)value;
        break;
        case REAL_ELEM:
            node->value.real = *(double*)value;
            free((double*)value);
        break;
        case STRING_ELEM:
            node->value.string = (uint8_t*)value;
        break;
        case LIST_ELEM:
            node->value.list = (struct List*)value;
        break;
        case TREE_ELEM:
            node->value.tree = (struct Tree*)value;
        break;
        case HASHTAB_ELEM:
            node->value.hashtab = (struct HashTab*)value;
        break;
        case STACK_ELEM: 
            node->value.stack = (struct Stack*)value;
        break;
        case BIGINT_ELEM:
            node->value.bigint = (struct BigInt*)value;
        break;
    }
    return node;
}

static void _print_list(vtype_t type, list_node *node) {
    printf("#L[ ");
    while(node != NULL) {
        switch(type) {
            case DECIMAL_ELEM:
                printf("%d", node->value.decimal);
            break;
            case REAL_ELEM:
                printf("%lf", node->value.real);
            break;
            case STRING_ELEM:
                printf("'%s'", node->value.string);
            break;
            case LIST_ELEM:
                print_list(node->value.list);
            break;
            case TREE_ELEM:
                print_tree(node->value.tree);
            break;
            case HASHTAB_ELEM:
                print_hashtab(node->value.hashtab);
            break;
            case STACK_ELEM: 
                print_stack(node->value.stack);
            break;
            case BIGINT_ELEM:
                print_bigint(node->value.bigint);
            break;
        }
        putchar(' ');
        node = node->next;
    }
    putchar(']');
}
