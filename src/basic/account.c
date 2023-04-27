#include "basic.h"

struct object_sub account_sub = {NULL, METHOD_ATTRIB &account_attrib};
struct object_tlv account_tlv = {METHOD_GET_TLV &account_get_tlv, METHOD_SET_TLV &account_set_tlv};
struct object_type account_type = {ACCOUNT_OP, &account_tlv, &account_sub};

// Standard operations
struct account_st *account_new() {
    struct account_st *res = skr_malloc(sizeof(struct account_st));
    string_data_init(&res->address);
    string_data_init(&res->login);

    integer_data_init(&res->activated);
    integer_data_init(&res->freeze);

    integer_data_init(&res->hash_type);
    integer_data_init(&res->crypto_type);
    integer_data_init(&res->crypto_base);
    return res;
}
void account_free(struct account_st *res) {
    if (res == NULL) return;
    string_data_free(&res->address);
    string_data_free(&res->login);

    integer_data_free(&res->activated);
    integer_data_free(&res->freeze);

    integer_data_free(&res->hash_type);
    integer_data_free(&res->crypto_type);
    integer_data_free(&res->crypto_base);
    skr_free(res);
}

void account_set(struct account_st *res, const struct account_st *a) {
    if (res == NULL) return;
    if (a == NULL) return account_clear(res);

    string_set(&res->address, &a->address);
    string_set(&res->login, &a->login);

    integer_set(&res->activated, &a->activated);
    integer_set(&res->freeze, &a->freeze);

    integer_set(&res->hash_type, &a->hash_type);
    integer_set(&res->crypto_type, &a->crypto_type);
    integer_set(&res->crypto_base, &a->crypto_base);
}
void account_copy(struct account_st *res, const struct account_st *a) {
    if (res == NULL) return;
    if (a == NULL) return account_clear(res);

    string_copy(&res->address, &a->address);
    string_copy(&res->login, &a->login);

    integer_copy(&res->activated, &a->activated);
    integer_copy(&res->freeze, &a->freeze);

    integer_copy(&res->hash_type, &a->hash_type);
    integer_copy(&res->crypto_type, &a->crypto_type);
    integer_copy(&res->crypto_base, &a->crypto_base);
}

void account_clear(struct account_st *res) {
    if (res == NULL) return;
    string_clear(&res->address);
    string_clear(&res->login);

    integer_clear(&res->activated);
    integer_clear(&res->freeze);

    integer_clear(&res->hash_type);
    integer_clear(&res->crypto_type);
    integer_clear(&res->crypto_base);
}
int account_cmp(const struct account_st *obj1, const struct account_st *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(&obj1->address, &obj2->address) != 0) return 2;
    return 0;
}

// Cmp Methods
int account_is_null(const struct account_st *a) {
    return (a == NULL || string_is_null(&a->address));
}

// TLV Methods
void account_set_tlv(struct account_st *res, const struct string_st *tlv) {
    if (res == NULL) return;
    account_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != TLV_ACCOUNT) return;

    char *data = tlv_get_value(tlv->data);
    struct string_st *_tlv = string_new();

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(&res->address, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(&res->login, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(&res->activated, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(&res->freeze, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(&res->hash_type, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(&res->crypto_type, _tlv);

    tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(&res->crypto_base, _tlv);

    string_free(_tlv);
}
void account_get_tlv(const struct account_st *account, struct string_st *res) {
    if (res == NULL) return;
    if (account == NULL) return string_clear(res);

    struct string_st *tlv = string_new();
    string_get_tlv(&account->address, res);

    string_get_tlv(&account->login, tlv);
    string_concat(res, tlv);

    integer_get_tlv(&account->activated, tlv);
    string_concat(res, tlv);

    integer_get_tlv(&account->freeze, tlv);
    string_concat(res, tlv);

    integer_get_tlv(&account->hash_type, tlv);
    string_concat(res, tlv);

    integer_get_tlv(&account->crypto_type, tlv);
    string_concat(res, tlv);

    integer_get_tlv(&account->crypto_base, tlv);
    string_concat(res, tlv);

    tlv_set_string(res, TLV_ACCOUNT, res);
    string_free(tlv);
}

// Attrib Methods
struct object_st *account_attrib
(struct object_st *err, const struct account_st *account, const struct string_st *str) {
    struct object_st *res = object_new();
    if (str->size == 7 && memcmp(str->data, "address", 7) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &account->address);
    }
    else if (str->size == 5 && memcmp(str->data, "login", 5) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &account->login);
    }
    else if (str->size == 9 && memcmp(str->data, "activated", 9) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &account->activated);
    }
    else if (str->size == 6 && memcmp(str->data, "freeze", 6) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &account->freeze);
    }
    else if (str->size == 9 && memcmp(str->data, "hash_type", 9) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &account->hash_type);
    }
    else if (str->size == 11 && memcmp(str->data, "crypto_type", 11) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &account->crypto_type);
    }
    else if (str->size == 11 && memcmp(str->data, "crypto_base", 11) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &account->crypto_base);
    }
    else {
        object_free(res);
        object_set_type(err, STRING_TYPE);
        string_set_str(err->data, "This Attribute does not exist", 29);
        return NULL;
    }
    return res;
}
