#include "restore.h"

struct restore_queue_st *restore_queue_new() {
    struct restore_queue_st *res = malloc(sizeof(struct restore_queue_st));
    res->front = NULL;
    res->back = NULL;
    res->size = 0;
    return res;
}
void restore_queue_free(struct restore_queue_st *res) {
    if (res == NULL) return;
    struct restore_queue_elm_st *elm = res->front;
    struct restore_queue_elm_st *next = NULL;
    while (elm != NULL) {
        next = elm->next;
        restore_queue_elm_free(elm);
        elm = next;
    }
    free(res);
}

void restore_queue_set(struct restore_queue_st *res, const struct restore_queue_st *a) {
    if (res == NULL) return;
    restore_queue_clear(res);
    if (a == NULL) return;

    struct restore_queue_elm_st *elm = a->front;
    while(elm != NULL) {
        restore_queue_push_back(res);
        restore_queue_elm_set(res->back, elm);
        elm = elm->next;
    }
}
void restore_queue_copy(struct restore_queue_st *res, const struct restore_queue_st *a) {
    if (res == NULL) return;
    restore_queue_clear(res);
    if (a == NULL) return;

    struct restore_queue_elm_st *elm = a->front;
    while(elm != NULL) {
        restore_queue_push_back(res);
        restore_queue_elm_copy(res->back, elm);
        elm = elm->next;
    }
}

void restore_queue_clear(struct restore_queue_st *res) {
    if (res == NULL) return;

    struct restore_queue_elm_st *elm = res->front;
    struct restore_queue_elm_st *next = NULL;
    while (elm != NULL) {
        next = elm->next;
        restore_queue_elm_free(elm);
        elm = next;
    }
    res->size = 0;

    res->front = NULL;
    res->back = NULL;
}
int8_t restore_queue_cmp(const struct restore_queue_st *obj1, const struct restore_queue_st *obj2) {
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    if (obj1->size > obj2->size) return CMP_GRET;
    if (obj1->size < obj2->size) return CMP_LESS;
    int8_t res_cmp_sub;
    struct restore_queue_elm_st *elm1 = obj1->front;
    struct restore_queue_elm_st *elm2 = obj2->front;
    while (elm1 != NULL && elm2 != NULL) {
        res_cmp_sub = hash_time_cmp(&elm1->hash_time, &elm2->hash_time);
        if (res_cmp_sub != CMP_EQ) return res_cmp_sub;
        elm1 = elm1->next;
        elm2 = elm2->next;
    }
    return CMP_EQ;
}

// Data Methods
void restore_queue_data_init(struct restore_queue_st *res) {
    if (res == NULL) return;
    res->front = NULL;
    res->back = NULL;
    res->size = 0;
}
void restore_queue_data_free(struct restore_queue_st *res) {
    if (res == NULL) return;
    struct restore_queue_elm_st *elm = res->front;
    struct restore_queue_elm_st *next = NULL;
    while (elm != NULL) {
        next = elm->next;
        restore_queue_elm_free(elm);
        elm = next;
    }
}

// Cmp Methods
int restore_queue_is_null(const struct restore_queue_st *res) {
    return (res == NULL || res->size == 0);
}
void restore_queue_push_back(struct restore_queue_st *res) {
    if (res == NULL) return;

    struct restore_queue_elm_st *elm = restore_queue_elm_new();
    res->size ++;
    if (res->size == 1) {
        res->back = res->front = elm;
    } else {
        res->back->next = elm;
        res->back = elm;
    }
}
void restore_queue_pop_front(struct restore_queue_st *res) {
    if (res == NULL || res->size == 0) return;

    struct restore_queue_elm_st *elm = res->front;
    res->size --;
    if (res->size == 0) {
        res->back = res->front = NULL;
    } else {
        res->front = elm->next;
        elm->next = NULL;
    }

    restore_queue_elm_free(elm);
}

// TLV Methods
int8_t restore_queue_set_tlv(struct restore_queue_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    restore_queue_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_LINKED_LIST) return ERR_TLV_TAG;

    struct restore_queue_elm_st elm;
    struct string_st _tlv, _tlv_data;
    restore_queue_elm_data_init(&elm);
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    result = tlv_get_value(tlv, &_tlv);

    for (; _tlv.size && result == 0;) {
        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        if ((result = restore_queue_elm_set_tlv(&elm, &_tlv_data))) break;
        restore_queue_push_back(res);
        restore_queue_elm_set(res->back, &elm);
    }

    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    restore_queue_elm_data_free(&elm);
    return result;
}
void restore_queue_get_tlv(const struct restore_queue_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    string_clear(tlv);
    if (res == NULL) return;

    struct string_st _tlv_data;
    struct restore_queue_elm_st *elm = res->front;
    string_data_init(&_tlv_data);

    while (elm != NULL) {
        restore_queue_elm_get_tlv(elm, &_tlv_data);
        string_concat(tlv, &_tlv_data);

        elm = elm->next;
    }
    tlv_set_string(tlv, TLV_LINKED_LIST, tlv);
    string_data_free(&_tlv_data);
}