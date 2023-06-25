#include "smart.h"


void closure_list_set(struct closure_list_st *res, const struct closure_list_st *a) {
    if(res->type) return;
    closure_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i++) res->closures[i] = a->closures[i];
}
void closure_list_clear(struct closure_list_st *res) {
    closure_list_resize(res, 0);
}

void closure_list_data_init(struct closure_list_st *res) {
    res->closures = NULL;
    res->max_size = 0;
    res->size = 0;
    res->type = 0;
}
void closure_list_data_free(struct closure_list_st *res) {
    closure_list_resize(res, 0);
    if (res->closures != NULL) free(res->closures);
}

void closure_list_resize(struct closure_list_st *res, size_t size) {
    if (res->closures == NULL && size != 0) {
        res->max_size = size;
        res->closures = malloc(sizeof(struct closure_st *) * size);
        for (size_t i = 0; i < size; i++) res->closures[i] = NULL;
    } else if (res->max_size < size) {
        res->closures = realloc(res->closures, sizeof(struct closure_st *) * size * 2);
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->closures[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->type && res->size > size) {
        for (size_t i = size, l = res->size; i < l; i++) {
            if (res->closures[i] != NULL) closure_free(res->closures[i]);
            res->closures[i] = NULL;
        }
    }
    res->size = size;
}
void closure_list_append(struct closure_list_st *res, struct closure_st *data) {
    closure_list_resize(res, res->size + 1);
    res->closures[res->size - 1] = data;
}
size_t closure_list_add_new(struct closure_list_st *res) {
    if(!res->type) return 0;
    closure_list_resize(res, res->size + 1);
    res->closures[res->size - 1] = closure_new();
    return res->size - 1;
}
struct closure_st *closure_list_last(struct closure_list_st *res) {
    if (res->closures == NULL || res->size == 0) return NULL;
    return res->closures[res->size - 1];
}

// TLV Methods
int closure_list_set_tlv(struct closure_list_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    closure_list_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_CLOSURE_LIST) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    result = tlv_get_value(tlv, &_tlv);

    for (; _tlv.size && result == 0;) {
        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        result = closure_set_tlv(res->closures[closure_list_add_new(res)], &_tlv_data);
    }

    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void closure_list_get_tlv(const struct closure_list_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    string_clear(tlv);
    if (res == NULL) return;

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    for (size_t i = 0, size = res->size; i < size; i++) {
        closure_get_tlv(res->closures[i], &_tlv_data);
        string_concat(tlv, &_tlv_data);
    }
    tlv_set_string(tlv, TLV_CLOSURE_LIST, tlv);
    string_data_free(&_tlv_data);
}