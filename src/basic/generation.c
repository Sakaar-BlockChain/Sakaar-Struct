#include "basic.h"

struct object_sub generation_sub = {NULL, METHOD_ATTRIB &generation_attrib};
struct object_tlv generation_tlv = {METHOD_GET_TLV &generation_get_tlv, METHOD_SET_TLV &generation_set_tlv};
struct object_type generation_type = {GENERATION_OP, &generation_tlv, &generation_sub};

// Standard operations
struct generation_st *generation_new() {
    struct generation_st *res = malloc(sizeof(struct currency_st));
    hash_time_data_init(&res->hash_time);
    string_data_init(&res->data);

    integer_set_time(&res->hash_time.time);
    return res;
}
void generation_free(struct generation_st *res) {
    if (res == NULL) return;
    hash_time_data_free(&res->hash_time);
    string_data_free(&res->data);
    free(res);
}

void generation_set(struct generation_st *res, const struct generation_st *a) {
    if (res == NULL) return;
    if (a == NULL) return generation_clear(res);

    hash_time_set(&res->hash_time, &a->hash_time);
    string_set(&res->data, &a->data);
}
void generation_copy(struct generation_st *res, const struct generation_st *a) {
    if (res == NULL) return;
    if (a == NULL) return generation_clear(res);

    hash_time_copy(&res->hash_time, &a->hash_time);
    string_copy(&res->data, &a->data);
}

void generation_clear(struct generation_st *res) {
    if (res == NULL) return;
    hash_time_clear(&res->hash_time);
    string_clear(&res->data);
}
int8_t generation_cmp(const struct generation_st *obj1, const struct generation_st *obj2) {
    if (obj1 == NULL || obj2 == NULL || hash_time_cmp(&obj1->hash_time, &obj2->hash_time)) return CMP_NEQ;
    return CMP_EQ;
}

// Data Methods
void generation_data_init(struct generation_st *res) {
    if (res == NULL) return;
    hash_time_data_init(&res->hash_time);
    string_data_init(&res->data);

    integer_set_time(&res->hash_time.time);
}
void generation_data_free(struct generation_st *res) {
    if (res == NULL) return;
    hash_time_data_free(&res->hash_time);
    string_data_free(&res->data);
}

// TLV Methods
int8_t generation_set_tlv(struct generation_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    generation_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_GENERATION) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = hash_time_set_tlv(&res->hash_time, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->data, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void generation_get_tlv(const struct generation_st *gen, struct string_st *res) {
    if (res == NULL) return;
    if (gen == NULL) return string_clear(res);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    hash_time_get_tlv(&gen->hash_time, res);

    string_get_tlv(&gen->data, &_tlv_data);
    string_concat(res, &_tlv_data);

    tlv_set_string(res, TLV_GENERATION, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *generation_attrib
(struct error_st *err, const struct generation_st *gen, const struct string_st *str) {
    struct object_st *res = object_new();
    if (res == NULL) {
        error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
        return NULL;
    }
    if (str->size == 4 && memcmp(str->data, "hash_time", 9) == 0) {
        object_set_type(res, HASH_TIME_TYPE);
        hash_time_set(res->data, &gen->hash_time);
    }
    else if (str->size == 4 && memcmp(str->data, "data", 4) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &gen->data);
    }
    else {
        object_clear(res);
        error_set_msg(err, ErrorType_Math, "This Attribute does not exist");
        return NULL;
    }
    return res;
}







