#include "basic.h"

struct object_sub wallet_data_sub = {NULL, METHOD_ATTRIB &wallet_data_attrib};
struct object_tlv wallet_data_tlv = {METHOD_GET_TLV &wallet_data_get_tlv, METHOD_SET_TLV &wallet_data_set_tlv};
struct object_type wallet_data_type = {WALLET_DATA_OP, &wallet_data_tlv, &wallet_data_sub};

// Standard operations
struct wallet_data *wallet_data_new() {
    struct wallet_data *res = malloc(sizeof(struct wallet_data));

    string_data_init(&res->address);
    string_data_init(&res->currency);
    string_data_init(&res->address_outside);

    string_data_init(&res->hash);
    string_data_init(&res->pre_hash);

    integer_data_init(&res->balance);
    integer_data_init(&res->pre_balance);

    string_data_init(&res->last_block_hash);
    integer_data_init(&res->last_block_time);
    return res;
}
void wallet_data_free(struct wallet_data *res) {
    if (res == NULL) return;
    string_data_free(&res->address);
    string_data_free(&res->currency);
    string_data_free(&res->address_outside);

    string_data_free(&res->hash);
    string_data_free(&res->pre_hash);

    integer_data_free(&res->balance);
    integer_data_free(&res->pre_balance);

    string_data_free(&res->last_block_hash);
    integer_data_free(&res->last_block_time);
    free(res);
}

void wallet_data_set(struct wallet_data *res, const struct wallet_data *a) {
    if (res == NULL) return;
    if (a == NULL) return wallet_data_clear(res);

    string_set(&res->address, &a->address);
    string_set(&res->currency, &a->currency);
    string_set(&res->address_outside, &a->address_outside);

    string_set(&res->hash, &a->hash);
    string_set(&res->pre_hash, &a->pre_hash);

    integer_set(&res->balance, &a->balance);
    integer_set(&res->pre_balance, &a->pre_balance);

    string_set(&res->last_block_hash, &a->last_block_hash);
    integer_set(&res->last_block_time, &a->last_block_time);
}
void wallet_data_copy(struct wallet_data *res, const struct wallet_data *a) {
    if (res == NULL) return;
    if (a == NULL) return wallet_data_clear(res);

    string_copy(&res->address, &a->address);
    string_copy(&res->currency, &a->currency);
    string_copy(&res->address_outside, &a->address_outside);

    string_copy(&res->hash, &a->hash);
    string_copy(&res->pre_hash, &a->pre_hash);

    integer_copy(&res->balance, &a->balance);
    integer_copy(&res->pre_balance, &a->pre_balance);

    string_copy(&res->last_block_hash, &a->last_block_hash);
    integer_copy(&res->last_block_time, &a->last_block_time);
}

void wallet_data_clear(struct wallet_data *res) {
    if (res == NULL) return;
    string_clear(&res->address);
    string_clear(&res->currency);
    string_clear(&res->address_outside);

    string_clear(&res->hash);
    string_clear(&res->pre_hash);

    integer_clear(&res->balance);
    integer_clear(&res->pre_balance);

    string_clear(&res->last_block_hash);
    integer_clear(&res->last_block_time);
}
int8_t wallet_data_cmp(const struct wallet_data *obj1, const struct wallet_data *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(&obj1->address, &obj2->address) || string_cmp(&obj1->currency, &obj2->currency)) return CMP_NEQ;
    return CMP_EQ;
}

// Data Methods
void wallet_data_data_init(struct wallet_data *res) {
    if (res == NULL) return;
    string_data_init(&res->address);
    string_data_init(&res->currency);
    string_data_init(&res->address_outside);

    string_data_init(&res->hash);
    string_data_init(&res->pre_hash);

    integer_data_init(&res->balance);
    integer_data_init(&res->pre_balance);

    string_data_init(&res->last_block_hash);
    integer_data_init(&res->last_block_time);
}
void wallet_data_data_free(struct wallet_data *res) {
    if (res == NULL) return;
    string_data_free(&res->address);
    string_data_free(&res->currency);
    string_data_free(&res->address_outside);

    string_data_free(&res->hash);
    string_data_free(&res->pre_hash);

    integer_data_free(&res->balance);
    integer_data_free(&res->pre_balance);

    string_data_free(&res->last_block_hash);
    integer_data_free(&res->last_block_time);
}

// TLV Methods
int8_t wallet_data_set_tlv(struct wallet_data *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    wallet_data_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_WALLET_DATA) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->address, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->currency, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->address_outside, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->hash, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->pre_hash, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->balance, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->pre_balance, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->last_block_hash, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->last_block_time, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void wallet_data_get_tlv(const struct wallet_data *data, struct string_st *res) {
    if (res == NULL) return;
    if (data == NULL) return string_clear(res);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    string_get_tlv(&data->address, res);

    string_get_tlv(&data->currency, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&data->address_outside, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&data->hash, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&data->pre_hash, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&data->balance, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&data->pre_balance, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&data->last_block_hash, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&data->last_block_time, &_tlv_data);
    string_concat(res, &_tlv_data);

    tlv_set_string(res, TLV_WALLET_DATA, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *wallet_data_attrib
(struct error_st *err, const struct wallet_data *data, const struct string_st *str) {
    struct object_st *res = object_new();
    if (res == NULL) {
        error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
        return NULL;
    }
    if (str->size == 7 && memcmp(str->data, "address", 7) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &data->address);
    }
    else if (str->size == 8 && memcmp(str->data, "currency", 8) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &data->currency);
    }
    else if (str->size == 15 && memcmp(str->data, "address_outside", 15) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &data->address_outside);
    }
    else if (str->size == 4 && memcmp(str->data, "hash", 4) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &data->hash);
    }
    else if (str->size == 8 && memcmp(str->data, "pre_hash", 8) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &data->pre_hash);
    }
    else if (str->size == 7 && memcmp(str->data, "balance", 7) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &data->balance);
    }
    else if (str->size == 11 && memcmp(str->data, "pre_balance", 11) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &data->pre_balance);
    }
    else if (str->size == 15 && memcmp(str->data, "last_block_hash", 15) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &data->last_block_hash);
    }
    else if (str->size == 15 && memcmp(str->data, "last_block_time", 15) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &data->last_block_time);
    }
    else {
        object_clear(res);
        error_set_msg(err, ErrorType_Math, "This Attribute does not exist");
        return NULL;
    }
    return res;
}
