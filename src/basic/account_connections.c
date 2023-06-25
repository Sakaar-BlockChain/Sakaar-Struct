#include "basic.h"

struct object_sub account_conn_sub = {NULL, METHOD_ATTRIB &account_connections_attrib};
struct object_tlv account_conn_tlv = {METHOD_GET_TLV &account_connections_get_tlv, METHOD_SET_TLV &account_connections_set_tlv};
struct object_type account_conn_type = {ACCOUNT_CONN_OP, &account_conn_tlv, &account_conn_sub};

// Standard operations
struct account_connections *account_connections_new() {
    struct account_connections *res = malloc(sizeof(struct account_connections));
    string_data_init(&res->address);
    string_data_init(&res->currency);
    address_list_data_init(&res->addresses);
    return res;
}
void account_connections_free(struct account_connections *res) {
    if (res == NULL) return;
    string_data_free(&res->address);
    string_data_free(&res->currency);
    address_list_data_free(&res->addresses);
    free(res);
}

void account_connections_set(struct account_connections *res, const struct account_connections *a) {
    if (res == NULL) return;
    if (a == NULL) return account_connections_clear(res);

    string_set(&res->address, &a->address);
    string_set(&res->currency, &a->currency);
    address_list_set(&res->addresses, &a->addresses);
}
void account_connections_copy(struct account_connections *res, const struct account_connections *a) {
    if (res == NULL) return;
    if (a == NULL) return account_connections_clear(res);

    string_copy(&res->address, &a->address);
    string_copy(&res->currency, &a->currency);
    address_list_copy(&res->addresses, &a->addresses);
}

void account_connections_clear(struct account_connections *res) {
    if (res == NULL) return;
    string_clear(&res->address);
    string_clear(&res->currency);
    address_list_clear(&res->addresses);
}
int account_connections_cmp(const struct account_connections *obj1, const struct account_connections *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(&obj1->address, &obj2->address) || string_cmp(&obj1->currency, &obj2->currency)) return CMP_NEQ;
    return CMP_EQ;
}

// Data Methods
void account_connections_data_init(struct account_connections *res) {
    if (res == NULL) return;
    string_data_init(&res->address);
    string_data_init(&res->currency);
    address_list_data_init(&res->addresses);
}
void account_connections_data_free(struct account_connections *res) {
    if (res == NULL) return;
    string_data_free(&res->address);
    string_data_free(&res->currency);
    address_list_data_free(&res->addresses);
}

// TLV Methods
int account_connections_set_tlv(struct account_connections *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    account_connections_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_ACCOUNT_CONN) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->address, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->currency, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = address_list_set_tlv(&res->addresses, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void account_connections_get_tlv(const struct account_connections *account_conn, struct string_st *res) {
    if (res == NULL) return;
    if (account_conn == NULL) return string_clear(res);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    string_get_tlv(&account_conn->address, res);

    string_get_tlv(&account_conn->currency, &_tlv_data);
    string_concat(res, &_tlv_data);

    address_list_get_tlv(&account_conn->addresses, &_tlv_data);
    string_concat(res, &_tlv_data);

    tlv_set_string(res, TLV_ACCOUNT_CONN, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *account_connections_attrib
(struct error_st *err, const struct account_connections *account_conn, const struct string_st *str) {
    struct object_st *res = object_new();
    if (str->size == 7 && memcmp(str->data, "address", 7) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &account_conn->address);
    }
    else if (str->size == 8 && memcmp(str->data, "currency", 8) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &account_conn->currency);
    }
    else if (str->size == 9 && memcmp(str->data, "addresses", 9) == 0) {
        object_set_type(res, ADDRESS_LIST_TYPE);
        address_list_set(res->data, &account_conn->addresses);
    }
    else {
        object_clear(res);
        error_set_msg(err, ErrorType_Math, "This Attribute does not exist");
        return NULL;
    }
    return res;
}
