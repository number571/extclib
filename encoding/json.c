#include <stdlib.h>
#include <stdint.h>

#include <cjson/cJSON.h>

#include "json.h"

static _Bool _isvalid_type(JSON *elem, vtype_t type);

extern JSON *new_json() {
    return cJSON_CreateObject();
}

extern void free_json(JSON *json) {
    cJSON_Delete(json);
}

extern JSON *set_json(JSON *json, char *key, void *value, vtype_t type) {
    switch(type) {
        case DECIMAL_TYPE:
            return cJSON_AddNumberToObject(json, key, (int32_t)(intptr_t)value);
        break;
        case REAL_TYPE: {
            double fval = *(double*)value;
            free((double*)value);
            return cJSON_AddNumberToObject(json, key, fval);
        }
        break;
        case STRING_TYPE:
            return cJSON_AddStringToObject(json, key, (char*)value);
        break;
        case ARRAY_TYPE:
            return cJSON_AddArrayToObject(json, key);
        break;
        case DYNAMIC_TYPE:
            cJSON_AddItemToObject(json, key, (JSON*)value);
        break;
        default: ;
    }
    return NULL;
}

extern JSON *get_json(JSON *json, char *key, vtype_t type) {
    JSON *elem = cJSON_GetObjectItemCaseSensitive(json, key);
    if (!_isvalid_type(elem, type)) {
        return NULL;
    }
    return elem;
}

extern size_t arrsize_json(JSON *array) {
    return cJSON_GetArraySize(array);
}

extern JSON *arritem_json(JSON *array, size_t index, vtype_t type) {
    JSON *elem = cJSON_GetArrayItem(array, index);
    if (!_isvalid_type(elem, type)) {
        return NULL;
    }
    return elem;
}

extern void arrpush_json(JSON *array, void *value, vtype_t type) {
    JSON *elem;
    switch(type) {
        case DECIMAL_TYPE:
            elem = cJSON_CreateNumber((int32_t)(intptr_t)value);
            cJSON_AddItemToArray(array, elem);
        break;
        case REAL_TYPE: {
            double fval = *(double*)value;
            free((double*)value);
            elem = cJSON_CreateNumber(fval);
            cJSON_AddItemToArray(array, elem);
        }
        break;
        case STRING_TYPE:
            elem = cJSON_CreateString((char*)value);
            cJSON_AddItemToArray(array, elem);
        break;
        case ARRAY_TYPE:
            elem = cJSON_CreateArray();
            cJSON_AddItemToArray(array, elem);
        break;
        case DYNAMIC_TYPE:
            cJSON_AddItemToArray(array, (JSON*)value);
        break;
        default: ;
    }
}

extern JSON *decode_json(char *str) {
    return cJSON_Parse(str);
}

extern char *encode_json(JSON *json) {
    return cJSON_Print(json);
}

extern char *string_json(JSON *json) {
    return json->valuestring;
}

extern int32_t decimal_json(JSON *json) {
    return json->valueint;
}

extern double real_json(JSON *json) {
    return json->valuedouble;
}

static _Bool _isvalid_type(JSON *elem, vtype_t type) {
    _Bool flag = 0;
    switch(type) {
        case DECIMAL_TYPE:
            flag = cJSON_IsNumber(elem);
        break;
        case REAL_TYPE:
            flag = cJSON_IsNumber(elem);
        break;
        case STRING_TYPE:
            flag = cJSON_IsString(elem);
        break;
        case ARRAY_TYPE:
            flag = cJSON_IsArray(elem);
        break;
        case DYNAMIC_TYPE:
            flag = cJSON_IsObject(elem);
        break;
        default: ;
    }
    return flag;
}
