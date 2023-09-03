#include "smart.h"

struct variable_st *variable_new(size_t position) {
    struct variable_st *res = malloc(sizeof(struct variable_st));
    string_data_init(&res->name);
    res->position = position;
    return res;
}
void variable_set(struct variable_st *res, const struct variable_st *a) {
    if (res == NULL) return;
    string_set(&res->name, &a->name);
    res->position = a->position;
}
void variable_clear(struct variable_st *res) {
    if (res == NULL) return;
    string_clear(&res->name);
    res->position = 0;
}
void variable_free(struct variable_st *res) {
    if (res == NULL) return;
    string_data_free(&res->name);
    free(res);
}

void variable_data_init(struct variable_st *res) {
    if (res == NULL) return;
    string_data_init(&res->name);
    res->position = 0;
}
void variable_data_free(struct variable_st *res) {
    if (res == NULL) return;
    string_data_free(&res->name);
}

// TLV Methods
int8_t variable_set_tlv(struct variable_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    variable_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_VARIABLE) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->name, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = size_set_tlv(&res->position, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void variable_get_tlv(const struct variable_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    if (res == NULL) return string_clear(tlv);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    string_get_tlv(&res->name, tlv);

    size_get_tlv(res->position, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    tlv_set_string(tlv, TLV_VARIABLE, tlv);
    string_data_free(&_tlv_data);
}