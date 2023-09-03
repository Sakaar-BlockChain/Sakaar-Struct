#include "smart.h"

struct bytecode_st *bytecode_new() {
    struct bytecode_st *res = malloc(sizeof (struct bytecode_st));
    res->data = NULL;
    res->command = NULL;

    res->closure = 0;
    res->variable = 0;

    res->size = 0;
    res->max_size = 0;
    return res;
}
void bytecode_clear(struct bytecode_st *res) {
    if (res == NULL) return;
    bytecode_resize(res, 0);

    res->closure = 0;
    res->variable = 0;
}
void bytecode_free(struct bytecode_st *res) {
    if (res == NULL) return;
    if (res->data != NULL) free(res->data);
    if (res->command != NULL) free(res->command);
    free(res);
}

void bytecode_resize(struct bytecode_st *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->max_size = size;
        res->data = malloc(size * sizeof(void *));
        res->command = malloc(size);
        if (res->data != NULL) for (size_t i = 0; i < size; i++) res->data[i] = 0;
        if (res->command != NULL) for (size_t i = 0; i < size; i++) res->command[i] = 0;
    } else if (res->max_size < size) {
        res->data = realloc(res->data, size * 2 * sizeof(void *));
        res->command = realloc(res->command, size * 2);
        if (res->data != NULL) for (size_t i = res->max_size, l = size * 2; i < l; i++) res->data[i] = 0;
        if (res->command != NULL) for (size_t i = res->max_size, l = size * 2; i < l; i++) res->command[i] = 0;
        res->max_size = size * 2;
    }
    if (res->size > size)
        for (size_t i = size, l = res->size; i < l; i++) {
            res->data[i] = 0;
            res->command[i] = 0;
        }
    res->size = size;
}
void bytecode_append(struct bytecode_st *res, char command, size_t data) {
    if (res == NULL) return;
    bytecode_resize(res, res->size + 1);
    res->command[res->size - 1] = command;
    res->data[res->size - 1] = data;
}

// TLV Methods
int8_t bytecode_set_tlv(struct bytecode_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    bytecode_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_BYTECODE) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = size_set_tlv(&res->closure, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = size_set_tlv(&res->variable, &_tlv_data))) goto end;

    size_t _tag, _data;
    for (; _tlv.size;) {
        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        if ((result = size_set_tlv(&_tag, &_tlv_data))) break;

        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        if ((result = size_set_tlv(&_data, &_tlv_data))) break;

        bytecode_append(res, (char) tag, _data);
    }

    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void bytecode_get_tlv(const struct bytecode_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    string_clear(tlv);
    if (res == NULL) return;

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);

    size_get_tlv(res->closure, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    size_get_tlv(res->variable, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    for (size_t i = 0, size = res->size; i < size; i++) {
        size_get_tlv(res->command[i], &_tlv_data);
        string_concat(tlv, &_tlv_data);

        size_get_tlv(res->data[i], &_tlv_data);
        string_concat(tlv, &_tlv_data);
    }
    tlv_set_string(tlv, TLV_BYTECODE, tlv);
    string_data_free(&_tlv_data);
}