#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "type.h"

typedef struct list_node {
    value_t value;
    struct list_node *next;
} list_node;

#include "list.h"

static list_node *_new_node(vtype_t type, void *value);
static void _print_list(vtype_t type, list_node *node);

extern List *new_list(vtype_t type) {
    switch(type) {
        case DECIMAL_ELEM: case REAL_ELEM: case STRING_ELEM:
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
        switch(list->type) {
            case DECIMAL_ELEM:
                if((int32_t)(intptr_t)value == node->value.decimal) {
                    return index;
                }
            break;
            case REAL_ELEM:
                if(*(double*)value == node->value.real) {
                    free((double*)value);
                    return index;
                }
            break;
            case STRING_ELEM:
                if(strcmp((uint8_t*)value, node->value.string) == 0) {
                    return index;
                }
            break;
        }
        node = node->next;
        ++index;
    }
    if(list->type == REAL_ELEM) {
        free((double*)value);
    }
    return -1;
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
    putchar('\n');
}

extern void free_list(List *list) {
    list_node *temp = NULL;
    list_node *node = list->node;
    while(node != NULL) {
        temp = node->next;
        free(node);
        node = temp;
    }
    free(list);
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
    }
    return node;
}

static void _print_list(vtype_t type, list_node *node) {
    printf("[ ");
    while(node != NULL) {
        switch(type) {
            case DECIMAL_ELEM:
                printf("%d ", node->value.decimal);
                break;
            case REAL_ELEM:
                printf("%lf ", node->value.real);
                break;
            case STRING_ELEM:
                printf("'%s' ", node->value.string);
                break;
        }
        node = node->next;
    }
    putchar(']');
}
