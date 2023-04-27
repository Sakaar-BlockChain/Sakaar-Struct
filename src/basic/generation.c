#include "basic.h"

struct object_sub generation_sub = {NULL, METHOD_ATTRIB &generation_attrib};
struct object_tlv generation_tlv = {METHOD_GET_TLV &generation_get_tlv, METHOD_SET_TLV &generation_set_tlv};
struct object_type generation_type = {GENERATION_OP, &generation_tlv, &generation_sub};

// Standard operations
struct generation *generation_new() {
    struct generation *res = skr_malloc(sizeof(struct currency_st));
    integer_data_init(&res->time);
    string_data_init(&res->hash);
    string_data_init(&res->data);

    integer_set_time(&res->time);
    return res;
}
void generation_free(struct generation *res) {
    if (res == NULL) return;
    integer_data_free(&res->time);
    string_data_free(&res->hash);
    string_data_free(&res->data);
    skr_free(res);
}

void generation_set(struct generation *res, const struct generation *a) {
    if (res == NULL) return;
    if (a == NULL) return generation_clear(res);

    integer_set(&res->time, &a->time);
    string_set(&res->hash, &a->hash);
    string_set(&res->data, &a->data);
}
void generation_copy(struct generation *res, const struct generation *a) {
    if (res == NULL) return;
    if (a == NULL) return generation_clear(res);

    integer_copy(&res->time, &a->time);
    string_copy(&res->hash, &a->hash);
    string_copy(&res->data, &a->data);
}

void generation_clear(struct generation *res) {
    if (res == NULL) return;
    integer_clear(&res->time);
    string_clear(&res->hash);
    string_clear(&res->data);
}
int generation_cmp(const struct generation *obj1, const struct generation *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(&obj1->hash, &obj2->hash) != 0 || integer_cmp(&obj1->time, &obj2->time) != 0) return 2;
    return 0;
}

// TLV Methods
void generation_set_tlv(struct generation *res, const struct string_st *tlv) {
    if (res == NULL) return;
    generation_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != TLV_GENERATION) return;

    char *data = tlv_get_value(tlv->data);
    struct string_st *_tlv = string_new();

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(&res->time, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    {
        struct integer_st *num = integer_new();
        integer_set_tlv(num, _tlv);
        integer_get_str(num, &res->hash);
        integer_free(num);
    }

    tlv_get_next_tlv(data, _tlv);
    string_set_tlv(&res->data, _tlv);

    string_free(_tlv);
}
void generation_get_tlv(const struct generation *gen, struct string_st *res) {
    if (res == NULL) return;
    if (gen == NULL) return string_clear(res);

    struct string_st *tlv = string_new();
    integer_get_tlv(&gen->time, res);

    {
        struct integer_st *num = integer_new();
        integer_set_str(num, &gen->hash);
        integer_get_tlv(num, tlv);
        integer_free(num);
    }
    string_concat(res, tlv);

    string_get_tlv(&gen->data, tlv);
    string_concat(res, tlv);

    tlv_set_string(res, TLV_GENERATION, res);
    string_free(tlv);
}

// Attrib Methods
struct object_st *generation_attrib
(struct object_st *err, const struct generation *gen, const struct string_st *str) {
    struct object_st *res = object_new();
    if (str->size == 4 && memcmp(str->data, "time", 4) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &gen->time);
    }
    else if (str->size == 4 && memcmp(str->data, "data", 4) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &gen->data);
    }
    else if (str->size == 4 && memcmp(str->data, "hash", 4) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &gen->hash);
    }
    else {
        object_free(res);
        object_set_type(err, STRING_TYPE);
        string_set_str(err->data, "This Attribute does not exist", 29);
        return NULL;
    }
    return res;
}







