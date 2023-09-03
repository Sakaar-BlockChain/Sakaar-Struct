#include "restore.h"

struct restore_list_elm_st *restore_list_elm_new() {
    struct restore_list_elm_st *res = malloc(sizeof(struct restore_list_elm_st));
    hash_time_data_init(&res->hash_time);
    string_data_init(&res->hash);
    return res;
}
void restore_list_elm_free(struct restore_list_elm_st *res) {
    if (res == NULL) return;
    hash_time_data_free(&res->hash_time);
    string_data_free(&res->hash);
    free(res);
}

void restore_list_elm_set(struct restore_list_elm_st *res, const struct restore_list_elm_st *a) {
    if (res == NULL) return;
    if (a == NULL) return restore_list_elm_clear(res);
    hash_time_set(&res->hash_time, &a->hash_time);
    string_set(&res->hash, &a->hash);
}
void restore_list_elm_copy(struct restore_list_elm_st *res, const struct restore_list_elm_st *a) {
    if (res == NULL) return;
    if (a == NULL) return restore_list_elm_clear(res);
    hash_time_copy(&res->hash_time, &a->hash_time);
    string_copy(&res->hash, &a->hash);
}

void restore_list_elm_clear(struct restore_list_elm_st *res) {
    if (res == NULL) return;
    hash_time_clear(&res->hash_time);
    string_clear(&res->hash);
}
int8_t restore_list_elm_cmp(const struct restore_list_elm_st *obj1, const struct restore_list_elm_st *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(&obj1->hash, &obj2->hash) || hash_time_cmp(&obj1->hash_time, &obj2->hash_time)) return CMP_NEQ;
    return CMP_EQ;
}

// Data Methods
void restore_list_elm_data_init(struct restore_list_elm_st *res) {
    if (res == NULL) return;
    hash_time_data_init(&res->hash_time);
    string_data_init(&res->hash);
}
void restore_list_elm_data_free(struct restore_list_elm_st *res) {
    if (res == NULL) return;
    hash_time_data_free(&res->hash_time);
    string_data_free(&res->hash);
}

// TLV Methods
int8_t restore_list_elm_set_tlv(struct restore_list_elm_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    restore_list_elm_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_RESTORE_LIST_ELM) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;
    
    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = hash_time_set_tlv(&res->hash_time, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->hash, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void restore_list_elm_get_tlv(const struct restore_list_elm_st *restore_list_elm, struct string_st *res) {
    if (res == NULL) return;
    if (restore_list_elm == NULL) return string_clear(res);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    hash_time_get_tlv(&restore_list_elm->hash_time, res);

    string_get_tlv(&restore_list_elm->hash, &_tlv_data);
    string_concat(res, &_tlv_data);

    tlv_set_string(res, TLV_RESTORE_LIST_ELM, res);
    string_data_free(&_tlv_data);
}