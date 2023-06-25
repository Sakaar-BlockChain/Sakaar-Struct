#include "smart.h"

struct closure_st *closure_new() {
    struct closure_st *res = malloc(sizeof(struct closure_st));
    variable_list_data_init(&res->attrib);
    variable_list_data_init(&res->data);
    return res;
}
void closure_set(struct closure_st *res, const struct closure_st *a) {
    variable_list_set(&res->attrib, &a->attrib);
    variable_list_set(&res->data, &a->data);
}
void closure_clear(struct closure_st *res) {
    variable_list_clear(&res->attrib);
    variable_list_clear(&res->data);
}
void closure_free(struct closure_st *res) {
    variable_list_data_free(&res->attrib);
    variable_list_data_free(&res->data);
    free(res);
}

void closure_data_init(struct closure_st *res) {
    variable_list_data_init(&res->attrib);
    variable_list_data_init(&res->data);
}
void closure_data_free(struct closure_st *res) {
    variable_list_data_free(&res->attrib);
    variable_list_data_free(&res->data);
}

void closure_append(struct closure_st *res, struct variable_st *data, struct variable_st *ptr) {
    variable_list_append(&res->attrib, data);
    variable_list_append(&res->data, ptr);
}

// TLV Methods
int closure_set_tlv(struct closure_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    closure_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_CLOSURE) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = variable_list_set_tlv(&res->attrib, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = variable_list_set_tlv(&res->data, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void closure_get_tlv(const struct closure_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    if (res == NULL) return string_clear(tlv);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    variable_list_get_tlv(&res->attrib, tlv);

    variable_list_get_tlv(&res->data, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    tlv_set_string(tlv, TLV_CLOSURE, tlv);
    string_data_free(&_tlv_data);
}