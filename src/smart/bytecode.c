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
int bytecode_set_tlv(struct bytecode_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    bytecode_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_BYTECODE) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    struct integer_st position;
    integer_data_init(&position);
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&position, &_tlv_data))) goto end;
    res->closure = integer_get_si(&position);

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&position, &_tlv_data))) goto end;
    res->variable = integer_get_si(&position);

    for (; _tlv.size;) {
        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        if ((result = integer_set_tlv(&position, &_tlv_data))) break;
        char tag = (char) integer_get_si(&position);

        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        if ((result = integer_set_tlv(&position, &_tlv_data))) break;
        size_t data = integer_get_si(&position);

        bytecode_append(res, tag, data);
    }

    end:
    integer_data_free(&position);
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
    struct integer_st position;
    integer_data_init(&position);

    integer_set_ui(&position, res->closure);
    integer_get_tlv(&position, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    integer_set_ui(&position, res->variable);
    integer_get_tlv(&position, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    for (size_t i = 0, size = res->size; i < size; i++) {
        integer_set_ui(&position, res->command[i]);
        integer_get_tlv(&position, &_tlv_data);
        string_concat(tlv, &_tlv_data);

        integer_set_ui(&position, res->data[i]);
        integer_get_tlv(&position, &_tlv_data);
        string_concat(tlv, &_tlv_data);
    }
    tlv_set_string(tlv, TLV_BYTECODE, tlv);
    string_data_free(&_tlv_data);
    integer_data_free(&position);
}