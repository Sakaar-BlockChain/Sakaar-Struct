#include "basic.h"

struct object_sub wallet_data_sub = {NULL, METHOD_ATTRIB &wallet_data_attrib};
struct object_tlv wallet_data_tlv = {METHOD_GET_TLV &wallet_data_get_tlv, METHOD_SET_TLV &wallet_data_set_tlv};
struct object_type wallet_data_type = {WALLET_DATA_OP, &wallet_data_tlv, &wallet_data_sub};

// Standard operations
struct wallet_data *wallet_data_new() {
    struct wallet_data *res = skr_malloc(sizeof(struct wallet_data));

    string_data_init(&res->address);
    string_data_init(&res->currency);
    string_data_init(&res->address_outside);

    string_data_init(&res->hash);
    string_data_init(&res->pre_hash);

    integer_data_init(&res->balance);
    integer_data_init(&res->pre_balance);
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
    skr_free(res);
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
}
int wallet_data_cmp(const struct wallet_data *obj1, const struct wallet_data *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(&obj1->address, &obj2->address) != 0 || string_cmp(&obj1->currency, &obj2->currency) != 0) return 2;
    return 0;
}

// TLV Methods
void wallet_data_set_tlv(struct wallet_data *res, const struct string_st *tlv) {
    if (res == NULL) return;
    wallet_data_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != TLV_WALLET_DATA) return;

    char *data = tlv_get_value(tlv->data);
    struct string_st *_tlv = string_new();


    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(&res->address, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(&res->currency, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(&res->address_outside, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    {
        struct integer_st *num = integer_new();
        integer_set_tlv(num, _tlv);
        integer_get_str(num, &res->hash);
        integer_free(num);
    }

    data = tlv_get_next_tlv(data, _tlv);
    {
        struct integer_st *num = integer_new();
        integer_set_tlv(num, _tlv);
        integer_get_str(num, &res->pre_hash);
        integer_free(num);
    }

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(&res->balance, _tlv);

    tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(&res->pre_balance, _tlv);

    string_free(_tlv);
}
void wallet_data_get_tlv(const struct wallet_data *data, struct string_st *res) {
    if (res == NULL) return;
    if (data == NULL) return string_clear(res);

    struct string_st *tlv = string_new();
    string_get_tlv(&data->address, res);

    string_get_tlv(&data->currency, tlv);
    string_concat(res, tlv);

    string_get_tlv(&data->address_outside, tlv);
    string_concat(res, tlv);

    {
        struct integer_st *num = integer_new();
        integer_set_str(num, &data->hash);
        integer_get_tlv(num, tlv);
        integer_free(num);
    }
    string_concat(res, tlv);

    {
        struct integer_st *num = integer_new();
        integer_set_str(num, &data->pre_hash);
        integer_get_tlv(num, tlv);
        integer_free(num);
    }
    string_concat(res, tlv);

    integer_get_tlv(&data->balance, tlv);
    string_concat(res, tlv);

    integer_get_tlv(&data->pre_balance, tlv);
    string_concat(res, tlv);

    tlv_set_string(res, TLV_WALLET_DATA, res);
    string_free(tlv);
}

// Attrib Methods
struct object_st *wallet_data_attrib
(struct object_st *err, const struct wallet_data *data, const struct string_st *str) {
    struct object_st *res = object_new();
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
    else {
        object_free(res);
        object_set_type(err, STRING_TYPE);
        string_set_str(err->data, "This Attribute does not exist", 29);
        return NULL;
    }
    return res;
}
