#ifndef EXTCLIB_JSON_H_
#define EXTCLIB_JSON_H_

#include <stdint.h>

#include "../types/type.h"

typedef struct cJSON JSON_e;

extern JSON_e *new_json();
extern void free_json(JSON_e *json);

extern JSON_e *set_json(JSON_e *json, char *key, void *value, vtype_t type);
extern JSON_e *get_json(JSON_e *json, char *key, vtype_t type);

extern size_t arrsize_json(JSON_e *array);
extern JSON_e *arritem_json(JSON_e *array, size_t index, vtype_t type);

extern JSON_e *decode_json(char *str);
extern char *encode_json(JSON_e *json);

extern char *string_json(JSON_e *json);
extern int32_t decimal_json(JSON_e *json);
extern double real_json(JSON_e *json);

#endif /* EXTCLIB_JSON_H_ */
