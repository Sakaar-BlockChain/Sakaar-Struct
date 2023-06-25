#include "smart.h"

struct variable_st *variable_new(size_t position) {
    struct variable_st *res = malloc(sizeof(struct variable_st));
    string_data_init(&res->name);
    res->position = position;
    return res;
}
void variable_set(struct variable_st *res, const struct variable_st *a) {
    string_set(&res->name, &a->name);
    res->position = a->position;
}
void variable_clear(struct variable_st *res) {
    string_clear(&res->name);
    res->position = 0;
}
void variable_free(struct variable_st *res) {
    string_data_free(&res->name);
    free(res);
}

void variable_data_init(struct variable_st *res) {
    string_data_init(&res->name);
    res->position = 0;
}
void variable_data_free(struct variable_st *res) {
    string_data_free(&res->name);
}

// TLV Methods
int variable_set_tlv(struct variable_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    variable_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_VARIABLE) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->name, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    {
        struct integer_st position;
        integer_data_init(&position);
        result = integer_set_tlv(&position, &_tlv_data);
        if (!result) res->position = integer_get_si(&position);
        integer_data_free(&position);
    }
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

    {
        struct integer_st position;
        integer_data_init(&position);
        integer_set_ui(&position, res->position);
        integer_get_tlv(&position, &_tlv_data);
        integer_data_free(&position);
    }
    string_concat(tlv, &_tlv_data);

    tlv_set_string(tlv, TLV_VARIABLE, tlv);
    string_data_free(&_tlv_data);
}