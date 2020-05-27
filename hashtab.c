#include <stdio.h>
#include <stdlib.h>

#include "type.h"
#include "tree.h"
#include "list.h"
#include "hashtab.h"
#include "stack.h"
#include "bigint.h"

typedef struct HashTab {
    struct {
        vtype_t key;
        vtype_t value;
    } type;
    size_t size;
    Tree **table;
} HashTab;

static uint32_t _strhash(uint8_t *s, size_t size);

extern HashTab *new_hashtab(size_t size, vtype_t key, vtype_t value) {
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
    HashTab *hashtab = (HashTab*)malloc(sizeof(HashTab));
    hashtab->table = (Tree**)malloc(size * sizeof_tree());
    for (size_t i = 0; i < size; ++i) {
        hashtab->table[i] = new_tree(key, value);
    }
    hashtab->size = size;
    hashtab->type.key = key;
    hashtab->type.value = value;
    return hashtab;
}

extern void del_hashtab(HashTab *hashtab, void *key) {
    uint32_t hash;
    switch(hashtab->type.key) {
        case DECIMAL_ELEM:
            hash = (uint32_t)(intptr_t)key % hashtab->size;
        break;
        case STRING_ELEM:
            hash = _strhash((uint8_t*)key, hashtab->size);
        break;
    }
    del_tree(hashtab->table[hash], key);
}

extern _Bool in_hashtab(HashTab *hashtab, void *key) {
    uint32_t hash;
    _Bool result;
    switch(hashtab->type.key) {
        case DECIMAL_ELEM:
            hash = (uint32_t)(intptr_t)key % hashtab->size;
        break;
        case STRING_ELEM:
            hash = _strhash((uint8_t*)key, hashtab->size);
        break;
    }
    result = in_tree(hashtab->table[hash], key);
    return result;
}

extern value_t get_hashtab(HashTab *hashtab, void *key) {
    uint32_t hash;
    value_t result;
    switch(hashtab->type.key) {
        case DECIMAL_ELEM:
            hash = (uint32_t)(intptr_t)key % hashtab->size;
        break;
        case STRING_ELEM:
            hash = _strhash((uint8_t*)key, hashtab->size);
        break;
    }
    result = get_tree(hashtab->table[hash], key);
    return result;
}

extern void set_hashtab(HashTab *hashtab, void *key, void *value) {
    uint32_t hash;
    switch(hashtab->type.key) {
        case DECIMAL_ELEM:
            hash = (uint32_t)(intptr_t)key % hashtab->size;
        break;
        case STRING_ELEM:
            hash = _strhash((uint8_t*)key, hashtab->size);
        break;
    }
    set_tree(hashtab->table[hash], key, value);
}

extern int8_t cmp_hashtab(HashTab *x, HashTab *y) {
    if (x->type.key != y->type.key) {
        return -1;
    }
    if (x->type.value != y->type.value) {
        return -2;
    }
    if (x->size != y->size) {
        return 2;
    }
    for (size_t i = 0; i < x->size; ++i) {
        if (size_tree(x->table[i]) != size_tree(y->table[i])) {
            return 1;
        }
        if (cmp_tree(x->table[i], y->table[i]) != 0) {
            return 1;
        }
    }
    return 0;
}

extern size_t size_hashtab(HashTab *hashtab) {
    return hashtab->size;
}

extern size_t sizeof_hashtab(void) {
    return sizeof(HashTab);
}

extern void free_hashtab(HashTab *hashtab) {
    for (size_t i = 0; i < hashtab->size; ++i) {
        free_tree(hashtab->table[i]);
    }
    free(hashtab->table);
    free(hashtab);
}

extern void print_hashtab(HashTab *hashtab) {
    printf("#H[ ");
    for (size_t i = 0; i < hashtab->size; ++i) {
        if (size_tree(hashtab->table[i]) == 0) {
            continue;
        }
        printf("(%d :: ", i);
        print_tree(hashtab->table[i]);
        printf(") ");
    }
    putchar(']');
}

extern void println_hashtab(HashTab *hashtab) {
    print_hashtab(hashtab);
    putchar('\n');
}

extern void print_hashtab_format(HashTab *hashtab) {
    printf("#H[\n");
    for (size_t i = 0; i < hashtab->size; ++i) {
        if (size_tree(hashtab->table[i]) == 0) {
            continue;
        }
        printf("\t(%d :: ", i);
        print_tree(hashtab->table[i]);
        printf(")\n");
    }
    putchar(']');
}

extern void println_hashtab_format(HashTab *hashtab) {
    print_hashtab_format(hashtab);
    putchar('\n');
}

static uint32_t _strhash(uint8_t *s, size_t size) {
    uint32_t hashval;
    for (hashval = 0; *s != '\0'; ++s) {
        hashval = *s + 31 * hashval;
    }
    return hashval % size;
}
