#include "basic.h"

struct object_tlv wallet_data_tlv = {METHOD_GET_TLV &wallet_data_get_tlv, METHOD_SET_TLV &wallet_data_set_tlv};
struct object_type wallet_data_type = {WALLET_DATA_OP, &wallet_data_tlv};
// Standard operations
struct wallet_data *wallet_data_new() {
    struct wallet_data *res = skr_malloc(WALLET_DATA_SIZE);

    res->address = string_new();
    res->currency = string_new();
    res->address_outside = string_new();

    res->hash = string_new();
    res->pre_hash = string_new();

    res->balance = integer_new();
    res->pre_balance = integer_new();
    return res;
}
void wallet_data_set(struct wallet_data *res, const struct wallet_data *a) {
    if (a == NULL) return wallet_data_clear(res);

    string_set(res->address, a->address);
    string_set(res->currency, a->currency);
    string_set(res->address_outside, a->address_outside);

    string_set(res->hash, a->hash);
    string_set(res->pre_hash, a->pre_hash);

    integer_set(res->balance, a->balance);
    integer_set(res->pre_balance, a->pre_balance);
}
void wallet_data_clear(struct wallet_data *res) {
    string_clear(res->address);
    string_clear(res->currency);
    string_clear(res->address_outside);

    string_clear(res->hash);
    string_clear(res->pre_hash);

    integer_clear(res->balance);
    integer_clear(res->pre_balance);
}
void wallet_data_free(struct wallet_data *res) {
    string_free(res->address);
    string_free(res->currency);
    string_free(res->address_outside);

    string_free(res->hash);
    string_free(res->pre_hash);

    integer_free(res->balance);
    integer_free(res->pre_balance);
    skr_free(res);
}

// TLV Methods
void wallet_data_set_tlv(struct wallet_data *res, const struct string_st *tlv) {
    if (res == NULL) return;
    wallet_data_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != TLV_WALLET_DATA) return;

    char *data = tlv_get_value(tlv->data);
    struct string_st *_tlv = string_new();


    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->address, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->currency, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->address_outside, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    {
        struct integer_st *num = integer_new();
        integer_set_tlv(num, _tlv);
        integer_get_str(num, res->hash);
        integer_free(num);
    }

    data = tlv_get_next_tlv(data, _tlv);
    {
        struct integer_st *num = integer_new();
        integer_set_tlv(num, _tlv);
        integer_get_str(num, res->pre_hash);
        integer_free(num);
    }

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->balance, _tlv);

    tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->pre_balance, _tlv);

    string_free(_tlv);
}
void wallet_data_get_tlv(const struct wallet_data *data, struct string_st *res) {
    if (res == NULL) return;
    if (data == NULL) return string_clear(res);

    struct string_st *tlv = string_new();
    string_get_tlv(data->address, res);

    string_get_tlv(data->currency, tlv);
    string_concat(res, tlv);

    string_get_tlv(data->address_outside, tlv);
    string_concat(res, tlv);

    {
        struct integer_st *num = integer_new();
        integer_set_str(num, data->hash);
        integer_get_tlv(num, tlv);
        integer_free(num);
    }
    string_concat(res, tlv);

    {
        struct integer_st *num = integer_new();
        integer_set_str(num, data->pre_hash);
        integer_get_tlv(num, tlv);
        integer_free(num);
    }
    string_concat(res, tlv);

    integer_get_tlv(data->balance, tlv);
    string_concat(res, tlv);

    integer_get_tlv(data->pre_balance, tlv);
    string_concat(res, tlv);

    tlv_set_string(res, TLV_WALLET_DATA, res);
    string_free(tlv);
}
