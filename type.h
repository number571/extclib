#ifndef EXTCLIB_TYPE_H_
#define EXTCLIB_TYPE_H_

typedef struct type_hashtab type_hashtab;

extern type_hashtab *type_hashtab_new(int size);
extern void type_hashtab_free(type_hashtab *ht);
extern int type_hashtab_size(type_hashtab *ht);

extern void *type_hashtab_select(type_hashtab *ht, char *key, int *size);
extern int type_hashtab_insert(type_hashtab *ht, char *key, void *elem, int size);
extern int type_hashtab_delete(type_hashtab *ht, char *key);

typedef struct type_list type_list;

extern type_list *type_list_new(void);
extern void type_list_free(type_list *ls);
extern int type_list_size(type_list *ls);

extern int type_list_find(type_list *ls, void *elem, int size);
extern void *type_list_select(type_list *ls, int index, int *size);
extern int type_list_insert(type_list *ls, int index, void *elem, int size);
extern int type_list_delete(type_list *ls, int index);

typedef struct type_stack type_stack;

extern type_stack *type_stack_new(int size, int valsize);
extern void type_stack_free(type_stack *st);
extern int type_stack_size(type_stack *st);

extern int type_stack_push(type_stack *st, void *elem);
extern void *type_stack_pop(type_stack *st);

extern int type_stack_set(type_stack *st, int index, void *elem);
extern void *type_stack_get(type_stack *st, int index);

#endif /* EXTCLIB_TYPE_H_ */
