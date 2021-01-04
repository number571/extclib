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

#endif /* EXTCLIB_TYPE_H_ */
