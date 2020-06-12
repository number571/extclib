#ifndef EXTCLIB_STRING_H_
#define EXTCLIB_STRING_H_

#include <stddef.h>
#include <stdint.h>

typedef struct String String;

extern String *new_string(char *str);
extern void free_string(String *string);

extern char get_string(String *string, size_t index);
extern _Bool eq_string(String *x, String *y);
extern int8_t cmp_string(String *x, String *y);
extern int8_t cmp_chars(char *x, char *y);
extern int8_t cmp_chars_string(String *x, char *y);

extern void cat_string(String *x, String *y);
extern void cpy_string(String *x, String *y);
extern int8_t catn_string(String *x, String *y, size_t begin, size_t quan);
extern int8_t cpyn_string(String *x, String *y, size_t begin, size_t quan);

extern void cat_out_string(char *x, String *y);
extern void cpy_out_string(char *x, String *y);
extern int8_t catn_out_string(char *x, String *y, size_t begin, size_t quan);
extern int8_t cpyn_out_string(char *x, String *y, size_t begin, size_t quan);

extern void cat_in_string(String *x, char *y);
extern void cpy_in_string(String *x, char *y);
extern int8_t catn_in_string(String *x, char *y, size_t quan);
extern int8_t cpyn_in_string(String *x, char *y, size_t quan);

extern size_t sizeof_string(void);
extern size_t len_string(String *string);
extern size_t cap_string(String *string);
extern size_t hash_string(String *string);

extern void print_string(String *string);
extern void println_string(String *string);

#endif /* EXTCLIB_STRING_H_ */
