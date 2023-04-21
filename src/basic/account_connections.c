#include "basic.h"

struct object_sub account_conn_sub = {NULL, METHOD_ATTRIB &account_connections_attrib};
struct object_tlv account_conn_tlv = {METHOD_GET_TLV &account_connections_get_tlv, METHOD_SET_TLV &account_connections_set_tlv};
struct object_type account_conn_type = {ACCOUNT_CONN_OP, &account_conn_tlv, &account_conn_sub};

// Standard operations
struct account_connections *account_connections_new() {
    struct account_connections *res = skr_malloc(sizeof(struct account_connections));
    res->address = string_new();
    res->currency = string_new();
    res->addresses = list_new();
    return res;
}
void account_connections_free(struct account_connections *res) {
    if (res == NULL) return;
    string_free(res->address);
    string_free(res->currency);
    list_free(res->addresses);
    skr_free(res);
}

void account_connections_set(struct account_connections *res, const struct account_connections *a) {
    if (res == NULL) return;
    if (a == NULL) return account_connections_clear(res);

    string_set(res->address, a->address);
    string_set(res->currency, a->currency);
    list_set(res->addresses, a->addresses);
}
void account_connections_copy(struct account_connections *res, const struct account_connections *a) {
    if (res == NULL) return;
    if (a == NULL) return account_connections_clear(res);

    string_copy(res->address, a->address);
    string_copy(res->currency, a->currency);
    list_copy(res->addresses, a->addresses);
}

void account_connections_clear(struct account_connections *res) {
    if (res == NULL) return;
    string_clear(res->address);
    string_clear(res->currency);
    list_clear(res->addresses);
}
int account_connections_cmp(const struct account_connections *obj1, const struct account_connections *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(obj1->address, obj2->address) != 0 || string_cmp(obj1->currency, obj2->currency) != 0) return 2;
    return 0;
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

// Attrib Methods
struct object_st *account_connections_attrib
(struct object_st *err, const struct account_connections *account_conn, const struct string_st *str) {
    struct object_st *res = object_new();
    if (str->size == 7 && memcmp(str->data, "address", 7) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, account_conn->address);
    }
    else if (str->size == 8 && memcmp(str->data, "currency", 8) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, account_conn->currency);
    }
    else if (str->size == 9 && memcmp(str->data, "addresses", 9) == 0) {
        object_set_type(res, LIST_TYPE);
        list_set(res->data, account_conn->addresses);
    }
    else {
        object_free(res);
        object_set_type(err, STRING_TYPE);
        string_set_str(err->data, "This Attribute does not exist", 29);
        return NULL;
    }
    return res;
}
