#include "smart.h"


struct variable_list_st *variable_list_new() {
    struct variable_list_st *res = skr_malloc(sizeof(struct variable_list_st));
    res->variables = NULL;
    res->max_size = 0;res->size = 0;
    res->type = 0;
    return res;
}
void variable_list_set(struct variable_list_st *res, const struct variable_list_st *a) {
    if(res->type) return;
    variable_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i++) res->variables[i] = a->variables[i];
}
void variable_list_clear(struct variable_list_st *res) {
    variable_list_resize(res, 0);
}
void variable_list_free(struct variable_list_st *res) {
    variable_list_resize(res, 0);
    if (res->variables != NULL) skr_free(res->variables);
    skr_free(res);
}

void variable_list_data_init(struct variable_list_st *res) {
    res->variables = NULL;
    res->max_size = 0;
    res->size = 0;
    res->type = 0;
}
void variable_list_data_free(struct variable_list_st *res) {
    variable_list_resize(res, 0);
    if (res->variables != NULL) skr_free(res->variables);
}

void variable_list_resize(struct variable_list_st *res, size_t size) {
    if (res->variables == NULL && size != 0) {
        res->max_size = size;
        res->variables = skr_malloc(sizeof(struct variable_st *) * size);
        for (size_t i = 0; i < size; i++) res->variables[i] = NULL;
    } else if (res->max_size < size) {
        res->variables = skr_realloc(res->variables, sizeof(struct variable_st *) * size * 2);
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->variables[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->type && res->size > size) {
        for (size_t i = size, l = res->size; i < l; i++) {
            if (res->variables[i] != NULL) variable_free(res->variables[i]);
            res->variables[i] = NULL;
        }
    }
    res->size = size;
}
void variable_list_append(struct variable_list_st *res, struct variable_st *data) {
    variable_list_resize(res, res->size + 1);
    res->variables[res->size - 1] = data;
}
size_t variable_list_add_new(struct variable_list_st *res) {
    if(!res->type) return 0;
    variable_list_resize(res, res->size + 1);
    res->variables[res->size - 1] = variable_new(res->size);
    return res->size - 1;
}
struct variable_st *variable_list_last(struct variable_list_st *res) {
    if (res->variables == NULL || res->size == 0) return NULL;
    return res->variables[res->size - 1];
}

// TLV Methods
int variable_list_set_tlv(struct variable_list_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    variable_list_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_VARIABLE_LIST) return ERR_TLV_TAG;

    struct string_st _tlv = {NULL, 0, 0}, _tlv_data  = {NULL, 0, 0};
    result = tlv_get_value(tlv, &_tlv);

    for (size_t pos; _tlv.size && result == 0;) {
        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        pos = variable_list_add_new(res);
        result = variable_set_tlv(res->variables[pos], &_tlv_data);
    }

    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void variable_list_get_tlv(const struct variable_list_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    string_clear(tlv);
    if (res == NULL) return;

    struct string_st _tlv_data = {NULL, 0, 0};
    for (size_t i = 0, size = res->size; i < size; i++)  {
        variable_get_tlv(res->variables[i], &_tlv_data);
        string_concat(tlv, &_tlv_data);
    }
    tlv_set_string(tlv, TLV_VARIABLE_LIST, tlv);
    string_data_free(&_tlv_data);
}