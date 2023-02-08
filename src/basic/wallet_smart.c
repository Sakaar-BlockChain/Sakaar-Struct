#include "basic.h"

struct object_tlv wallet_smart_tlv = {METHOD_GET_TLV &wallet_smart_get_tlv, METHOD_SET_TLV &wallet_smart_set_tlv};
struct object_type wallet_smart_type = {WALLET_SMART_OP, &wallet_smart_tlv};
// Standard operations
struct wallet_smart *wallet_smart_new() {
    struct wallet_smart *res = skr_malloc(WALLET_SMART_SIZE);
    res->address = string_new();
    res->currency = string_new();
    res->private_key = string_new();
    res->smart_private = string_new();
    res->smart_contract = string_new();
    res->name = string_new();
    res->owner = string_new();

    res->freeze = integer_new();
    return res;
}
void wallet_smart_set(struct wallet_smart *res, const struct wallet_smart *a) {
    if (a == NULL) return wallet_smart_clear(res);

    string_set(res->address, a->address);
    string_set(res->currency, a->currency);
    string_set(res->private_key, a->private_key);
    string_set(res->smart_private, a->smart_private);
    string_set(res->smart_contract, a->smart_contract);
    string_set(res->name, a->name);
    string_set(res->owner, a->owner);

    integer_set(res->freeze, a->freeze);
}
void wallet_smart_clear(struct wallet_smart *res) {
    string_clear(res->address);
    string_clear(res->currency);
    string_clear(res->private_key);
    string_clear(res->smart_private);
    string_clear(res->smart_contract);
    string_clear(res->name);
    string_clear(res->owner);

    integer_clear(res->freeze);
}
void wallet_smart_free(struct wallet_smart *res) {
    string_free(res->address);
    string_free(res->currency);
    string_free(res->private_key);
    string_free(res->smart_private);
    string_free(res->smart_contract);
    string_free(res->name);
    string_free(res->owner);

    integer_free(res->freeze);
    skr_free(res);
}

// TLV Methods
void wallet_smart_set_tlv(struct wallet_smart *res, const struct string_st *tlv) {
    if (res == NULL) return;
    wallet_smart_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != TLV_WALLET_SMART) return;

    char *data = tlv_get_value(tlv->data);
    struct string_st *_tlv = string_new();


    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->address, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->currency, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->private_key, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->smart_private, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->smart_contract, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->name, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->owner, _tlv);

    tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->freeze, _tlv);

    string_free(_tlv);
}
void wallet_smart_get_tlv(const struct wallet_smart *smart, struct string_st *res) {
    if (res == NULL) return;
    if (smart == NULL) return string_clear(res);

    struct string_st *tlv = string_new();
    string_get_tlv(smart->address, res);

    string_get_tlv(smart->currency, tlv);
    string_concat(res, tlv);

    string_get_tlv(smart->private_key, tlv);
    string_concat(res, tlv);

    string_get_tlv(smart->smart_private, tlv);
    string_concat(res, tlv);

    string_get_tlv(smart->smart_contract, tlv);
    string_concat(res, tlv);

    string_get_tlv(smart->name, tlv);
    string_concat(res, tlv);

    string_get_tlv(smart->owner, tlv);
    string_concat(res, tlv);

    integer_get_tlv(smart->freeze, tlv);
    string_concat(res, tlv);

    tlv_set_string(res, TLV_WALLET_SMART, res);
    string_free(tlv);
}
