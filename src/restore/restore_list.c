#include "restore.h"

// Standard operations
struct restore_list_st *restore_list_new() {
    struct restore_list_st *res = malloc(sizeof(struct restore_list_st));
    res->data = NULL;
    res->max_size = 0;
    res->size = 0;
    return res;
}
void restore_list_free(struct restore_list_st *res) {
    if (res == NULL) return;

    restore_list_resize(res, 0);
    if (res->data != NULL) free(res->data);
    free(res);
}

void restore_list_set(struct restore_list_st *res, const struct restore_list_st *a) {
    if (res == NULL) return;
    if (a == NULL) return restore_list_clear(res);

    restore_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) restore_list_elm_set(res->data[i], a->data[i]);
}
void restore_list_copy(struct restore_list_st *res, const struct restore_list_st *a) {
    if (res == NULL) return;
    if (a == NULL) return restore_list_clear(res);

    restore_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) restore_list_elm_copy(res->data[i], a->data[i]);
}

void restore_list_clear(struct restore_list_st *res) {
    if (res == NULL) return;
    restore_list_resize(res, 0);
}
int8_t restore_list_cmp(const struct restore_list_st *obj1, const struct restore_list_st *obj2) {
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    if (obj1->size > obj2->size) return CMP_GRET;
    if (obj1->size < obj2->size) return CMP_LESS;
    int8_t res_cmp_sub;
    for (size_t i = 0, size = obj1->size; i < size; i++) {
        res_cmp_sub = restore_list_elm_cmp(obj1->data[i], obj2->data[i]);
        if (res_cmp_sub != CMP_EQ) return res_cmp_sub;
    }
    return CMP_EQ;
}

// Data Methods
void restore_list_data_init(struct restore_list_st *res) {
    if (res == NULL) return;
    res->data = NULL;
    res->max_size = 0;
    res->size = 0;
}
void restore_list_data_free(struct restore_list_st *res) {
    if (res == NULL) return;
    restore_list_resize(res, 0);
    if (res->data != NULL) free(res->data);
}

// Cmp Methods
int restore_list_is_null(const struct restore_list_st *res) {
    return (res == NULL || res->size == 0);
}
void restore_list_resize(struct restore_list_st *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->max_size = size;
        res->data = malloc(sizeof(struct restore_list_elm_st *) * size);
        for (size_t i = 0; i < size; i++) res->data[i] = NULL;
    } else if (res->max_size < size) {
        res->data = realloc(res->data, sizeof(struct restore_list_elm_st *) * size * 2);
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->data[i] = NULL;
        res->max_size = size * 2;
    }
    for (size_t i = size, l = res->size; i < l; i++) {
        if (res->data[i] != NULL) restore_list_elm_free(res->data[i]);
        res->data[i] = NULL;
    }
    for (size_t i = res->size, l = size; i < l; i++) {
        if (res->data[i] == NULL) res->data[i] = restore_list_elm_new();
    }
    res->size = size;
}
void restore_list_find(const struct restore_list_st *nodes, struct restore_stack_st *stack, const struct hash_time_st *hash_time, size_t height, size_t *pos, size_t *pos_line) {
    restore_stack_clear(stack);
    if (pos != NULL) *pos = 0;
    if (pos_line != NULL) *pos_line = 0;
    if (hash_time_is_null(hash_time) || nodes == NULL || height == 0) return;

    size_t _pos = 0;
    size_t _pos_line = 0;

    struct restore_stack_elm_st *elm = NULL;
    size_t part_size;
    size_t sub_size;

    do {
        restore_stack_new_front(stack);
        if (stack != NULL) elm = stack->front;

        part_size = ((1 << (RESTORE_BLOCK_BIN * height)) - 1) / (RESTORE_BLOCK_CHILD - 1);
        sub_size = ((1 << (RESTORE_BLOCK_BIN * (height - 1))) - 1) / (RESTORE_BLOCK_CHILD - 1);

        while (_pos + part_size <= nodes->size &&
               !hash_time_is_null(&nodes->data[_pos + part_size - 1]->hash_time) &&
               hash_time_cmp(&nodes->data[_pos + part_size - 1]->hash_time, hash_time) < 0) {
            _pos += part_size;
            _pos_line += part_size - sub_size;
            if (stack != NULL) {
                string_concat(&elm->hash, &nodes->data[_pos - 1]->hash);
                elm->count++;
            }
        }

        height--;
    } while (part_size != 1);

    if (pos != NULL) *pos = _pos;
    if (pos_line != NULL) *pos_line = _pos_line;
}


// TLV Methods
int8_t restore_list_set_tlv(struct restore_list_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    restore_list_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_RESTORE_LIST) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    result = tlv_get_value(tlv, &_tlv);

    for (size_t pos; _tlv.size && result == 0;) {
        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        pos = res->size;
        restore_list_resize(res, pos + 1);
        result = restore_list_elm_set_tlv(res->data[pos], &_tlv_data);
    }

    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void restore_list_get_tlv(const struct restore_list_st *restore_list, struct string_st *res) {
    if (res == NULL) return;
    string_clear(res);
    if (restore_list == NULL) return;

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    for (size_t i = 0, size = restore_list->size; i < size; i++)  {
        restore_list_elm_get_tlv(restore_list->data[i], &_tlv_data);
        string_concat(res, &_tlv_data);
    }
    tlv_set_string(res, TLV_RESTORE_LIST, res);
    string_data_free(&_tlv_data);
}

