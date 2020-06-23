#ifndef EXTCLIB_JSON_H_
#define EXTCLIB_JSON_H_

#include <stdint.h>

#include "../types/type.h"

typedef struct cJSON JSON;

extern JSON *new_json();
extern void free_json(JSON *json);

extern JSON *set_json(JSON *json, char *key, void *value, vtype_t type);
extern JSON *get_json(JSON *json, char *key, vtype_t type);

extern size_t arrsize_json(JSON *array);
extern JSON *arritem_json(JSON *array, size_t index, vtype_t type);

extern JSON *decode_json(char *str);
extern char *encode_json(JSON *json);

extern char *string_json(JSON *json);
extern int32_t decimal_json(JSON *json);
extern double real_json(JSON *json);

#endif /* EXTCLIB_JSON_H_ */
