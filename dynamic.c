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

typedef struct Dynamic {
    vtype_t type;
    value_t value;
} Dynamic;

static void _free_dynamic(Dynamic *dynamic);

extern Dynamic *new_dynamic(char ctype, void *value) {
    Dynamic *dynamic;
    vtype_t type;
    switch(ctype) {
        case 'd': case 'r': case 's': case 'L': 
        case 'T': case 'H': case 'A': case 'B': 
            break;
        default:
            fprintf(stderr, "%s\n", "type not supported");
            return NULL;
    }
    dynamic = (Dynamic*)malloc(sizeof(Dynamic));
    switch(ctype) {
        case 'd': 
            dynamic->type = DECIMAL_ELEM; 
            dynamic->value.decimal = (int32_t)(intptr_t)value;
        break;
        case 'r': 
            dynamic->type = REAL_ELEM;  
            dynamic->value.real = *(double*)value;
            free((double*)value);  
        break;
        case 's': 
            dynamic->type = STRING_ELEM;  
            dynamic->value.string = (uint8_t*)value;
        break;
        case 'L': 
            dynamic->type = LIST_ELEM;    
            dynamic->value.list = (struct List*)value;
        break;
        case 'T': 
            dynamic->type = TREE_ELEM;    
            dynamic->value.tree = (struct Tree*)value;
        break;
        case 'H': 
            dynamic->type = HASHTAB_ELEM; 
            dynamic->value.hashtab = (struct HashTab*)value;
        break;
        case 'A': 
            dynamic->type = ARRAY_ELEM;   
            dynamic->value.array = (struct Array*)value;
        break;
        case 'B': 
            dynamic->type = BIGINT_ELEM;  
            dynamic->value.bigint = (struct BigInt*)value;
        break;
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
        case DECIMAL_ELEM:
            flag = x->value.decimal == y->value.decimal;
        break;
        case REAL_ELEM:
            flag = x->value.real == y->value.real;
        break;
        case STRING_ELEM:
            flag = strcmp(x->value.string, y->value.string) == 0;
        break;
        case LIST_ELEM:
            flag = cmp_list(x->value.list, y->value.list) == 0;
        break;
        case TREE_ELEM:
            flag = cmp_tree(x->value.tree, y->value.tree) == 0;
        break;
        case HASHTAB_ELEM:
            flag = cmp_hashtab(x->value.hashtab, y->value.hashtab) == 0;
        break;
        case ARRAY_ELEM:
            flag = cmp_array(x->value.array, y->value.array) == 0;
        break;
        case BIGINT_ELEM:
            flag = cmp_bigint(x->value.bigint, y->value.bigint) == 0;
        break;
    }
    return !flag;
}

extern void print_dynamic(Dynamic *dynamic) {
    switch(dynamic->type) {
        case DECIMAL_ELEM:
            printf("%d", dynamic->value.decimal);
        break;
        case REAL_ELEM:
            printf("%lf", dynamic->value.real);
        break;
        case STRING_ELEM:
            printf("'%s'", dynamic->value.string);
        break;
        case LIST_ELEM:
            print_list(dynamic->value.list);
        break;
        case TREE_ELEM:
            print_tree(dynamic->value.tree);
        break;
        case HASHTAB_ELEM:
            print_hashtab(dynamic->value.hashtab);
        break;
        case ARRAY_ELEM:
            print_array(dynamic->value.array);
        break;
        case BIGINT_ELEM:
            print_bigint(dynamic->value.bigint);
        break;
    }
}

extern void println_dynamic(Dynamic *dynamic) {
    print_dynamic(dynamic);
    putchar('\n');
}

static void _free_dynamic(Dynamic *dynamic) {
    switch(dynamic->type) {
        case LIST_ELEM:
            free_list(dynamic->value.list);
        break;
        case TREE_ELEM:
            free_tree(dynamic->value.tree);
        break;
        case HASHTAB_ELEM:
            free_hashtab(dynamic->value.hashtab);
        break;
        case ARRAY_ELEM: 
            free_array(dynamic->value.array);
        break;
        case BIGINT_ELEM:
            free_bigint(dynamic->value.bigint);
        break;
    }
}
