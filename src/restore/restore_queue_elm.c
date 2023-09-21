#include "restore.h"

struct restore_queue_elm_st *restore_queue_elm_new() {
    struct restore_queue_elm_st *res = malloc(sizeof(struct restore_queue_elm_st));
    hash_time_data_init(&res->hash_time);
    res->gen = NULL;
    res->typo = 0;

    res->next = NULL;
    return res;
}
void restore_queue_elm_free(struct restore_queue_elm_st *res) {
    if (res == NULL) return;
    hash_time_data_free(&res->hash_time);
    if (res->gen != NULL) generation_free(res->gen);
    free(res);
}

void restore_queue_elm_set(struct restore_queue_elm_st *res, const struct restore_queue_elm_st *a) {
    if (res == NULL) return;
    if (a == NULL) return restore_queue_elm_clear(res);
    hash_time_set(&res->hash_time, &a->hash_time);
    if (a->gen != NULL) {
        if (res->gen == NULL) res->gen = generation_new();
        generation_set(res->gen, a->gen);
    } else if (res->gen != NULL) generation_clear(res->gen);
    res->typo = a->typo;
}
void restore_queue_elm_copy(struct restore_queue_elm_st *res, const struct restore_queue_elm_st *a) {
    if (res == NULL) return;
    if (a == NULL) return restore_queue_elm_clear(res);
    hash_time_copy(&res->hash_time, &a->hash_time);
    if (a->gen != NULL) {
        if (res->gen == NULL) res->gen = generation_new();
        generation_copy(res->gen, a->gen);
    } else if (res->gen != NULL) generation_clear(res->gen);
    res->typo = a->typo;
}

void restore_queue_elm_clear(struct restore_queue_elm_st *res) {
    if (res == NULL) return;
    hash_time_data_free(&res->hash_time);
    if (res->gen != NULL) generation_free(res->gen);
    res->gen = NULL;
    res->typo = 0;
}

// Data Methods
void restore_queue_elm_data_init(struct restore_queue_elm_st *res) {
    if (res == NULL) return;
    hash_time_data_init(&res->hash_time);
    res->typo = 0;
    res->gen = NULL;

    res->next = NULL;
}
void restore_queue_elm_data_free(struct restore_queue_elm_st *res) {
    if (res == NULL) return;
    hash_time_data_free(&res->hash_time);
    if (res->gen != NULL) generation_free(res->gen);
}

// TLV Methods
int8_t restore_queue_elm_set_tlv(struct restore_queue_elm_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    restore_queue_elm_clear(res);
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

    if (res->gen == NULL) res->gen = generation_new();
    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = generation_set_tlv(res->gen, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void restore_queue_elm_get_tlv(const struct restore_queue_elm_st *restore_queue_elm, struct string_st *res) {
    if (res == NULL) return;
    if (restore_queue_elm == NULL) return string_clear(res);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    hash_time_get_tlv(&restore_queue_elm->hash_time, res);

    generation_get_tlv(restore_queue_elm->gen, &_tlv_data);
    string_concat(res, &_tlv_data);

    tlv_set_string(res, TLV_RESTORE_LIST_ELM, res);
    string_data_free(&_tlv_data);
}