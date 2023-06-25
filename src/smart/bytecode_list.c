#include "smart.h"

void bytecode_list_set(struct bytecode_list_st *res, const struct bytecode_list_st *a) {
    if(res->type) return;
    bytecode_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i++) res->bytecodes[i] = a->bytecodes[i];
}
void bytecode_list_clear(struct bytecode_list_st *res) {
    bytecode_list_resize(res, 0);
}

void bytecode_list_data_init(struct bytecode_list_st *res) {
    res->bytecodes = NULL;
    res->max_size = 0;
    res->size = 0;
    res->type = 0;
}
void bytecode_list_data_free(struct bytecode_list_st *res) {
    bytecode_list_resize(res, 0);
    if (res->bytecodes != NULL) free(res->bytecodes);
}

void bytecode_list_resize(struct bytecode_list_st *res, size_t size) {
    if (res->bytecodes == NULL && size != 0) {
        res->max_size = size;
        res->bytecodes = malloc(sizeof(struct bytecode_st *) * size);
        for (size_t i = 0; i < size; i++) res->bytecodes[i] = NULL;
    } else if (res->max_size < size) {
        res->bytecodes = realloc(res->bytecodes, sizeof(struct bytecode_st *) * size * 2);
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->bytecodes[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->type && res->size > size) {
        for (size_t i = size, l = res->size; i < l; i++) {
            if (res->bytecodes[i] != NULL) bytecode_free(res->bytecodes[i]);
            res->bytecodes[i] = NULL;
        }
    }
    res->size = size;
}
void bytecode_list_append(struct bytecode_list_st *res, struct bytecode_st *data) {
    bytecode_list_resize(res, res->size + 1);
    res->bytecodes[res->size - 1] = data;
}
size_t bytecode_list_add_new(struct bytecode_list_st *res) {
    if(!res->type) return 0;
    bytecode_list_resize(res, res->size + 1);
    res->bytecodes[res->size - 1] = bytecode_new();
    return res->size - 1;
}
struct bytecode_st *bytecode_list_last(struct bytecode_list_st *res) {
    if (res->bytecodes == NULL || res->size == 0) return NULL;
    return res->bytecodes[res->size - 1];
}
struct bytecode_st *bytecode_list_pop(struct bytecode_list_st *res) {
    if (res->type || res->bytecodes == NULL || res->size == 0) return NULL;
    return res->bytecodes[--res->size];
}

// TLV Methods
int bytecode_list_set_tlv(struct bytecode_list_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    bytecode_list_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_BYTECODE_LIST) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    result = tlv_get_value(tlv, &_tlv);

    for (size_t pos; _tlv.size && result == 0;) {
        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        pos = bytecode_list_add_new(res);
        result = bytecode_set_tlv(res->bytecodes[pos], &_tlv_data);
    }

    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void bytecode_list_get_tlv(const struct bytecode_list_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    string_clear(tlv);
    if (res == NULL) return;

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    for (size_t i = 0, size = res->size; i < size; i++) {
        bytecode_get_tlv(res->bytecodes[i], &_tlv_data);
        string_concat(tlv, &_tlv_data);
    }
    tlv_set_string(tlv, TLV_BYTECODE_LIST, tlv);
    string_data_free(&_tlv_data);
}