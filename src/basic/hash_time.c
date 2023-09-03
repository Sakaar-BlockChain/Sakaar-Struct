#include "basic.h"

struct object_sub hash_time_sub = {NULL, METHOD_ATTRIB &hash_time_attrib};
struct object_tlv hash_time_tlv = {METHOD_GET_TLV &hash_time_get_tlv, METHOD_SET_TLV &hash_time_set_tlv};
struct object_type hash_time_type = {HASH_TIME_OP, &hash_time_tlv, &hash_time_sub};

// Standard operations
struct hash_time_st *hash_time_new() {
    struct hash_time_st *res = malloc(sizeof(struct hash_time_st));
    string_data_init(&res->hash);
    integer_data_init(&res->time);
    return res;
}
void hash_time_free(struct hash_time_st *res) {
    if (res == NULL) return;
    string_data_free(&res->hash);
    integer_data_free(&res->time);
    free(res);
}

void hash_time_set(struct hash_time_st *res, const struct hash_time_st *a) {
    if (res == NULL) return;
    if (a == NULL) return hash_time_clear(res);

    string_set(&res->hash, &a->hash);
    integer_set(&res->time, &a->time);
}
void hash_time_copy(struct hash_time_st *res, const struct hash_time_st *a) {
    if (res == NULL) return;
    if (a == NULL) return hash_time_clear(res);

    string_copy(&res->hash, &a->hash);
    integer_copy(&res->time, &a->time);
}

void hash_time_clear(struct hash_time_st *res) {
    if (res == NULL) return;
    string_clear(&res->hash);
    integer_clear(&res->time);
}
int8_t hash_time_cmp(const struct hash_time_st *obj1, const struct hash_time_st *obj2) {
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    int8_t result = integer_cmp(&obj1->time, &obj2->time);
    if (result) return result;
    return string_cmp(&obj1->hash, &obj2->hash);
}

// Cmp Methods
int8_t hash_time_is_null(const struct hash_time_st *res) {
    return (int8_t) (res == NULL || string_is_null(&res->hash) || integer_is_null(&res->time));
}
int8_t hash_time_get_string(const struct hash_time_st *res, struct string_st *str) {
    if (str == NULL) return ERR_DATA_NULL;
    string_clear(str);
    if (res == NULL) return ERR_DATA_NULL;

    struct string_st temp;

    string_data_init(&temp);

    integer_get_str(&res->time, &temp);
    string_concat(str, &temp);
    string_set_str(&temp, "_", 1);
    string_concat(str, &temp);
    string_concat(str, &res->hash);

    string_data_free(&temp);
    return ERR_SUCCESS;
}

// Data Methods
void hash_time_data_init(struct hash_time_st *res) {
    if (res == NULL) return;
    string_data_init(&res->hash);
    integer_data_init(&res->time);
}
void hash_time_data_free(struct hash_time_st *res) {
    if (res == NULL) return;
    string_data_free(&res->hash);
    integer_data_free(&res->time);
}

// TLV Methods
int8_t hash_time_set_tlv(struct hash_time_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    hash_time_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_HASH_TIME) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->hash, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->time, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void hash_time_get_tlv(const struct hash_time_st *hash_time, struct string_st *res) {
    if (res == NULL) return;
    if (hash_time == NULL) return string_clear(res);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    string_get_tlv(&hash_time->hash, res);

    integer_get_tlv(&hash_time->time, &_tlv_data);
    string_concat(res, &_tlv_data);

    tlv_set_string(res, TLV_HASH_TIME, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *hash_time_attrib
        (struct error_st *err, const struct hash_time_st *hash_time, const struct string_st *str) {
    struct object_st *res = object_new();
    if (res == NULL) {
        error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
        return NULL;
    }
    if (str->size == 7 && memcmp(str->data, "hash", 4) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &hash_time->hash);
    }
    else if (str->size == 9 && memcmp(str->data, "time", 4) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &hash_time->time);
    }
    else {
        object_clear(res);
        error_set_msg(err, ErrorType_Math, "This Attribute does not exist");
        return NULL;
    }
    return res;
}
