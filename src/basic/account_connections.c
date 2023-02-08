#include "basic.h"

struct object_tlv account_conn_tlv = {METHOD_GET_TLV &account_connections_get_tlv, METHOD_SET_TLV &account_connections_set_tlv};
struct object_type account_conn_type = {ACCOUNT_CONN_OP, &account_conn_tlv};
// Standard operations
struct account_connections *account_connections_new() {
    struct account_connections *res = skr_malloc(ACCOUNT_CONN_SIZE);
    res->address = string_new();
    res->currency = string_new();
    res->addresses = list_new();
    return res;
}
void account_connections_set(struct account_connections *res, const struct account_connections *a) {
    if (a == NULL) return account_connections_clear(res);

    string_set(res->address, a->address);
    string_set(res->currency, a->currency);
    list_set(res->addresses, a->addresses);
}
void account_connections_clear(struct account_connections *res) {
    string_clear(res->address);
    string_clear(res->currency);
    list_clear(res->addresses);
}
void account_connections_free(struct account_connections *res) {
    string_free(res->address);
    string_free(res->currency);
    list_free(res->addresses);
    skr_free(res);
}

// TLV Methods
void account_connections_set_tlv(struct account_connections *res, const struct string_st *tlv) {
    if (res == NULL) return;
    account_connections_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != TLV_ACCOUNT_CONN) return;

    char *data = tlv_get_value(tlv->data);
    struct string_st *_tlv = string_new();

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->address, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->currency, _tlv);

    tlv_get_next_tlv(data, _tlv);
    list_set_tlv_self(res->addresses, _tlv, STRING_TYPE);

    string_free(_tlv);
}
void account_connections_get_tlv(const struct account_connections *account_conn, struct string_st *res) {
    if (res == NULL) return;
    if (account_conn == NULL) return string_clear(res);

    struct string_st *tlv = string_new();
    string_get_tlv(account_conn->address, res);

    string_get_tlv(account_conn->currency, tlv);
    string_concat(res, tlv);

    list_get_tlv(account_conn->addresses, tlv);
    string_concat(res, tlv);

    tlv_set_string(res, TLV_ACCOUNT_CONN, res);
    string_free(tlv);
}