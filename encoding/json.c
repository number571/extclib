#include <stdlib.h>
#include <stdint.h>

#include <cjson/cJSON.h>

#include "json.h"

static _Bool _isvalid_type(JSON_e *elem, vtype_t type);

extern JSON_e *new_json() {
    return (JSON_e*)cJSON_CreateObject();
}

extern void free_json(JSON_e *json) {
    cJSON_Delete((cJSON*)json);
}

extern JSON_e *set_json(JSON_e *json, char *key, void *value, vtype_t type) {
    switch(type) {
        case DECIMAL_TYPE:
            return cJSON_AddNumberToObject((cJSON*)json, key, (int32_t)(intptr_t)value);
        break;
        case REAL_TYPE: {
            double fval = *(double*)value;
            free((double*)value);
            return cJSON_AddNumberToObject((cJSON*)json, key, fval);
        }
        break;
        case STRING_TYPE:
            return cJSON_AddStringToObject((cJSON*)json, key, (char*)value);
        break;
        case ARRAY_TYPE:
            return cJSON_AddArrayToObject((cJSON*)json, key);
        break;
        case DYNAMIC_TYPE:
            cJSON_AddItemToObject((cJSON*)json, key, (cJSON*)value);
        break;
        default: ;
    }
    return NULL;
}

extern JSON_e *get_json(JSON_e *json, char *key, vtype_t type) {
    JSON_e *elem = (JSON_e*)cJSON_GetObjectItemCaseSensitive(json, key);
    if (!_isvalid_type(elem, type)) {
        return NULL;
    }
    return elem;
}

extern size_t arrsize_json(JSON_e *array) {
    return cJSON_GetArraySize((cJSON*)array);
}

extern JSON_e *arritem_json(JSON_e *array, size_t index, vtype_t type) {
    JSON_e *elem = (JSON_e*)cJSON_GetArrayItem((cJSON*)array, index);
    if (!_isvalid_type(elem, type)) {
        return NULL;
    }
    return elem;
}

extern JSON_e *decode_json(char *str) {
    return (JSON_e*)cJSON_Parse(str);
}

extern char *encode_json(JSON_e *json) {
    return cJSON_Print((cJSON*)json);
}

extern char *string_json(JSON_e *json) {
    return json->valuestring;
}

extern int32_t decimal_json(JSON_e *json) {
    return json->valueint;
}

extern double real_json(JSON_e *json) {
    return json->valuedouble;
}

static _Bool _isvalid_type(JSON_e *elem, vtype_t type) {
    _Bool flag = 0;
    switch(type) {
        case DECIMAL_TYPE:
            flag = cJSON_IsNumber((cJSON*)elem);
        break;
        case REAL_TYPE:
            flag = cJSON_IsNumber((cJSON*)elem);
        break;
        case STRING_TYPE:
            flag = cJSON_IsString((cJSON*)elem);
        break;
        case ARRAY_TYPE:
            flag = cJSON_IsArray((cJSON*)elem);
        break;
        case DYNAMIC_TYPE:
            flag = cJSON_IsObject((cJSON*)elem);
        break;
        default: ;
    }
    return flag;
}
