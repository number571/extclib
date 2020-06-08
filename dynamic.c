#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "type.h"
#include "list.h"
#include "tree.h"
#include "hashtab.h"
#include "array.h"
#include "bigint.h"
#include "dynamic.h"
#include "string.h"

typedef struct Dynamic {
    vtype_t type;
    value_t value;
} Dynamic;

static void _free_dynamic(Dynamic *dynamic);

extern Dynamic *new_dynamic(vtype_t type, void *value) {
    Dynamic *dynamic;
    switch(type) {
        case DECIMAL_TYPE: 
        case REAL_TYPE: 
        case CHARS_TYPE: 
        case LIST_TYPE: 
        case TREE_TYPE: 
        case HASHTAB_TYPE: 
        case ARRAY_TYPE:
        case BIGINT_TYPE:
        case STRING_TYPE:
            break;
        default:
            fprintf(stderr, "%s\n", "type not supported");
            return NULL;
    }
    dynamic = (Dynamic*)malloc(sizeof(Dynamic));
    dynamic->type = type;
    switch(type) {
        case DECIMAL_TYPE:
            dynamic->value.decimal = (int32_t)(intptr_t)value;
        break;
        case REAL_TYPE: 
            dynamic->value.real = *(double*)value;
            free((double*)value);  
        break;
        case CHARS_TYPE: 
            dynamic->value.chars = (uint8_t*)value;
        break;
        case LIST_TYPE: 
            dynamic->value.list = (struct List*)value;
        break;
        case TREE_TYPE:  
            dynamic->value.tree = (struct Tree*)value;
        break;
        case HASHTAB_TYPE: 
            dynamic->value.hashtab = (struct HashTab*)value;
        break;
        case ARRAY_TYPE: 
            dynamic->value.array = (struct Array*)value;
        break;
        case BIGINT_TYPE: 
            dynamic->value.bigint = (struct BigInt*)value;
        break;
        case STRING_TYPE: 
            dynamic->value.string = (struct String*)value;
        break;
        default: ;
    }
    return dynamic;
}

extern void free_dynamic(Dynamic *dynamic) {
    _free_dynamic(dynamic);
    free(dynamic);
}

extern size_t sizeof_dynamic(void) {
    return sizeof(Dynamic);
}

extern value_t value_dynamic(Dynamic *dynamic) {
    return dynamic->value;
}

extern vtype_t type_dynamic(Dynamic *dynamic) {
    return dynamic->type;
}

extern int8_t cmp_dynamic(Dynamic *x, Dynamic *y) {
    if (x->type != y->type) {
        return -1;
    }
    _Bool flag = 0;
    switch(x->type) {
        case DECIMAL_TYPE:
            flag = x->value.decimal == y->value.decimal;
        break;
        case REAL_TYPE:
            flag = x->value.real == y->value.real;
        break;
        case CHARS_TYPE:
            flag = strcmp((char*)x->value.chars, (char*)y->value.chars) == 0;
        break;
        case LIST_TYPE:
            flag = cmp_list(x->value.list, y->value.list) == 0;
        break;
        case TREE_TYPE:
            flag = cmp_tree(x->value.tree, y->value.tree) == 0;
        break;
        case HASHTAB_TYPE:
            flag = cmp_hashtab(x->value.hashtab, y->value.hashtab) == 0;
        break;
        case ARRAY_TYPE:
            flag = cmp_array(x->value.array, y->value.array) == 0;
        break;
        case BIGINT_TYPE:
            flag = cmp_bigint(x->value.bigint, y->value.bigint) == 0;
        break;
        case STRING_TYPE:
            flag = cmp_string(x->value.string, y->value.string) == 0;
        break;
        default: ;
    }
    return !flag;
}

extern void print_dynamic(Dynamic *dynamic) {
    switch(dynamic->type) {
        case DECIMAL_TYPE:
            printf("%d", dynamic->value.decimal);
        break;
        case REAL_TYPE:
            printf("%lf", dynamic->value.real);
        break;
        case CHARS_TYPE:
            printf("'%s'", dynamic->value.chars);
        break;
        case LIST_TYPE:
            print_list(dynamic->value.list);
        break;
        case TREE_TYPE:
            print_tree(dynamic->value.tree);
        break;
        case HASHTAB_TYPE:
            print_hashtab(dynamic->value.hashtab);
        break;
        case ARRAY_TYPE:
            print_array(dynamic->value.array);
        break;
        case BIGINT_TYPE:
            print_bigint(dynamic->value.bigint);
        break;
        case STRING_TYPE:
            print_string(dynamic->value.string);
        break;
        default: ;
    }
}

extern void println_dynamic(Dynamic *dynamic) {
    print_dynamic(dynamic);
    putchar('\n');
}

static void _free_dynamic(Dynamic *dynamic) {
    switch(dynamic->type) {
        case LIST_TYPE:
            free_list(dynamic->value.list);
        break;
        case TREE_TYPE:
            free_tree(dynamic->value.tree);
        break;
        case HASHTAB_TYPE:
            free_hashtab(dynamic->value.hashtab);
        break;
        case ARRAY_TYPE: 
            free_array(dynamic->value.array);
        break;
        case BIGINT_TYPE:
            free_bigint(dynamic->value.bigint);
        break;
        case STRING_TYPE:
            free_string(dynamic->value.string);
        break;
        default: ;
    }
}
