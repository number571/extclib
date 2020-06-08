#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "type.h"
#include "list.h"
#include "tree.h"
#include "hashtab.h"
#include "array.h"
#include "bigint.h"
#include "dynamic.h"
#include "string.h"

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
static void _print_list(List *list, vtype_t type, list_node *node);
static void _free_list(List *list, list_node *node);

extern List *new_list(vtype_t type) {
    switch(type) {
        case DECIMAL_TYPE: 
        case REAL_TYPE: 
        case CHARS_TYPE: 
        case LIST_TYPE: 
        case TREE_TYPE: 
        case HASHTAB_TYPE: 
        case ARRAY_TYPE:
        case BIGINT_TYPE:
        case DYNAMIC_TYPE:
        case STRING_TYPE:
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
            case DECIMAL_TYPE:
                flag = (int32_t)(intptr_t)value == node->value.decimal;
            break;
            case REAL_TYPE:
                flag = *(double*)value == node->value.real;
                if(flag) {
                    free((double*)value);
                }
            break;
            case CHARS_TYPE:
                flag = strcmp((char*)value, (char*)node->value.chars) == 0;
            break;
            case LIST_TYPE:
                flag = cmp_list((List*)value, node->value.list) == 0;
            break;
            case TREE_TYPE:
                flag = cmp_tree((Tree*)value, node->value.tree) == 0;
            break;
            case HASHTAB_TYPE:
                flag = cmp_hashtab((HashTab*)value, node->value.hashtab) == 0;
            break;
            case ARRAY_TYPE:
                flag = cmp_array((Array*)value, node->value.array) == 0;
            break;
            case BIGINT_TYPE:
                flag = cmp_bigint((BigInt*)value, node->value.bigint) == 0;
            break;
            case DYNAMIC_TYPE:
                flag = cmp_dynamic((Dynamic*)value, node->value.dynamic) == 0;
            break;
            case STRING_TYPE:
                flag = cmp_string((String*)value, node->value.string) == 0;
            break;
        }
        if (flag) {
            return index;
        }
        node = node->next;
        ++index;
    }
    if(list->type == REAL_TYPE) {
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
        _Bool flag = 0;
        switch(x->type) {
            case DECIMAL_TYPE:
                flag = ptrx->value.decimal == ptry->value.decimal;
            break;
            case REAL_TYPE:
                flag = ptrx->value.real == ptry->value.real;
            break;
            case CHARS_TYPE:
                flag = strcmp((char*)ptrx->value.chars, (char*)ptry->value.chars) == 0;
            break;
            case LIST_TYPE:
                flag = cmp_list(ptrx->value.list, ptry->value.list) == 0;
            break;
            case TREE_TYPE:
                flag = cmp_tree(ptrx->value.tree, ptry->value.tree) == 0;
            break;
            case HASHTAB_TYPE:
                flag = cmp_hashtab(ptrx->value.hashtab, ptry->value.hashtab) == 0;
            break;
            case ARRAY_TYPE:
                flag = cmp_array(ptrx->value.array, ptrx->value.array) == 0;
            break;
            case BIGINT_TYPE:
                flag = cmp_bigint(ptrx->value.bigint, ptrx->value.bigint) == 0;
            break;
            case DYNAMIC_TYPE:
                flag = cmp_dynamic(ptrx->value.dynamic, ptrx->value.dynamic) == 0;
            break;
            case STRING_TYPE:
                flag = cmp_string(ptrx->value.string, ptrx->value.string) == 0;
            break;
        }
        if (!flag) {
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
        value_t none = {
            .decimal = 0,
        };
        return none;
    }
    if(node == NULL) {
        fprintf(stderr, "%s\n", "undefined");
        value_t none = {
            .decimal = 0,
        };
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
    if ((list->type == LIST_TYPE && (List*)value == list) ||
        (list->type == DYNAMIC_TYPE && 
            type_dynamic((Dynamic*)value) == LIST_TYPE && 
            value_dynamic((Dynamic*)value).list == list)) {
        return -1;
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
    _free_list(list, curr_node);
    free(curr_node);
    list->size -= 1;
    return 0;
}

extern int8_t push_list(List *list, void *value) {
    if ((list->type == LIST_TYPE && (List*)value == list) ||
        (list->type == DYNAMIC_TYPE && 
            type_dynamic((Dynamic*)value) == LIST_TYPE && 
            value_dynamic((Dynamic*)value).list == list)) {
        return -1;
    }
    if(list->node == NULL) {
        list->node = _new_node(list->type, value);
        list->size += 1;
        return 0;
    }
    list_node *node = list->node;
    while(node->next != NULL) {
        node = node->next;
    }
    node->next = _new_node(list->type, value);
    list->size += 1;
    return 0;
}

extern value_t pop_list(List *list) {
    list_node *curr_node = list->node;
    if(curr_node == NULL) {
        fprintf(stderr, "%s\n", "stack overflow");
        value_t none = {
            .decimal = 0,
        };
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
    _print_list(list, list->type, list->node);
}

extern void println_list(List *list) {
    _print_list(list, list->type, list->node);
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
        case LIST_TYPE:
            free_list(node->value.list);
        break;
        case TREE_TYPE:
            free_tree(node->value.tree);
        break;
        case HASHTAB_TYPE:
            free_hashtab(node->value.hashtab);
        break;
        case ARRAY_TYPE: 
            free_array(node->value.array);
        break;
        case BIGINT_TYPE:
            free_bigint(node->value.bigint);
        break;
        case DYNAMIC_TYPE:
            free_dynamic(node->value.dynamic);
        break;
        case STRING_TYPE:
            free_string(node->value.string);
        break;
        default: ;
    }
}

static list_node *_new_node(vtype_t type, void *value) {
    list_node *node = (list_node*)malloc(sizeof(list_node));
    node->next = NULL;
    switch(type) {
        case DECIMAL_TYPE:
            node->value.decimal = (int32_t)(intptr_t)value;
        break;
        case REAL_TYPE:
            node->value.real = *(double*)value;
            free((double*)value);
        break;
        case CHARS_TYPE:
            node->value.chars = (uint8_t*)value;
        break;
        case LIST_TYPE:
            node->value.list = (struct List*)value;
        break;
        case TREE_TYPE:
            node->value.tree = (struct Tree*)value;
        break;
        case HASHTAB_TYPE:
            node->value.hashtab = (struct HashTab*)value;
        break;
        case ARRAY_TYPE: 
            node->value.array = (struct Array*)value;
        break;
        case BIGINT_TYPE:
            node->value.bigint = (struct BigInt*)value;
        break;
        case DYNAMIC_TYPE:
            node->value.dynamic = (struct Dynamic*)value;
        break;
        case STRING_TYPE:
            node->value.string = (struct String*)value;
        break;
    }
    return node;
}

static void _print_list(List *list, vtype_t type, list_node *node) {
    printf("#L[ ");
    while(node != NULL) {
        switch(type) {
            case DECIMAL_TYPE:
                printf("%d", node->value.decimal);
            break;
            case REAL_TYPE:
                printf("%lf", node->value.real);
            break;
            case CHARS_TYPE:
                printf("'%s'", node->value.chars);
            break;
            case LIST_TYPE:
                print_list(node->value.list);
            break;
            case TREE_TYPE:
                print_tree(node->value.tree);
            break;
            case HASHTAB_TYPE:
                print_hashtab(node->value.hashtab);
            break;
            case ARRAY_TYPE: 
                print_array(node->value.array);
            break;
            case BIGINT_TYPE:
                print_bigint(node->value.bigint);
            break;
            case DYNAMIC_TYPE:
                print_dynamic(node->value.dynamic);
            break;
            case STRING_TYPE:
                print_string(node->value.string);
            break;
        }
        putchar(' ');
        node = node->next;
    }
    putchar(']');
}
