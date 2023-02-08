#include "basic.h"

struct object_tlv wallet_tlv = {METHOD_GET_TLV &wallet_get_tlv, METHOD_SET_TLV &wallet_set_tlv};
struct object_type wallet_type = {WALLET_OP, &wallet_tlv};
// Standard operations
struct wallet_st *wallet_new() {
    struct wallet_st *res = skr_malloc(WALLET_SIZE);
    res->data = wallet_data_new();
    res->smart = wallet_smart_new();
    return res;
}
void wallet_set(struct wallet_st *res, const struct wallet_st *a) {
    if (a == NULL) return wallet_clear(res);

    wallet_data_set(res->data, a->data);
    wallet_smart_set(res->smart, a->smart);
}
void wallet_clear(struct wallet_st *res) {
    wallet_data_clear(res->data);
    wallet_smart_clear(res->smart);
}
void wallet_free(struct wallet_st *res) {
    wallet_data_free(res->data);
    wallet_smart_free(res->smart);
    skr_free(res);
}

// TLV Methods
void wallet_set_tlv(struct wallet_st *res, const struct string_st *tlv) {
    if (res == NULL) return;
    wallet_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != TLV_WALLET) return;

    char *data = tlv_get_value(tlv->data);
    struct string_st *_tlv = string_new();

    data = tlv_get_next_tlv(data, _tlv);
    wallet_data_set_tlv(res->data, _tlv);

    tlv_get_next_tlv(data, _tlv);
    wallet_smart_set_tlv(res->smart, _tlv);

    string_free(_tlv);
}
void wallet_get_tlv(const struct wallet_st *wallet, struct string_st *res) {
    if (res == NULL) return;
    if (wallet == NULL) return string_clear(res);

    struct string_st *tlv = string_new();
    wallet_data_get_tlv(wallet->data, res);

    wallet_smart_get_tlv(wallet->smart, tlv);
    string_concat(res, tlv);

    tlv_set_string(res, TLV_WALLET, res);
    string_free(tlv);
}
