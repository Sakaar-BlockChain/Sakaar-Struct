#include "basic.h"

struct object_sub account_sub = {NULL, METHOD_ATTRIB &account_attrib};
struct object_tlv account_tlv = {METHOD_GET_TLV &account_get_tlv, METHOD_SET_TLV &account_set_tlv};
struct object_type account_type = {ACCOUNT_OP, &account_tlv, &account_sub};

// Standard operations
struct account_st *account_new() {
    struct account_st *res = malloc(sizeof(struct account_st));
    string_data_init(&res->address);
    string_data_init(&res->login);

    res->activated = 0;
    integer_data_init(&res->freeze);

    res->hash_type = 0;
    res->crypto_type = 0;
    res->crypto_base = 0;
    return res;
}
void account_free(struct account_st *res) {
    if (res == NULL) return;
    string_data_free(&res->address);
    string_data_free(&res->login);

    integer_data_free(&res->freeze);
    free(res);
}

void account_set(struct account_st *res, const struct account_st *a) {
    if (res == NULL) return;
    if (a == NULL) return account_clear(res);

    string_set(&res->address, &a->address);
    string_set(&res->login, &a->login);

    res->activated = a->activated;
    integer_set(&res->freeze, &a->freeze);

    res->hash_type = a->hash_type;
    res->crypto_type = a->crypto_type;
    res->crypto_base = a->crypto_base;
}
void account_copy(struct account_st *res, const struct account_st *a) {
    if (res == NULL) return;
    if (a == NULL) return account_clear(res);

    string_copy(&res->address, &a->address);
    string_copy(&res->login, &a->login);

    res->activated = a->activated;
    integer_copy(&res->freeze, &a->freeze);

    res->hash_type = a->hash_type;
    res->crypto_type = a->crypto_type;
    res->crypto_base = a->crypto_base;
}

void account_clear(struct account_st *res) {
    if (res == NULL) return;
    string_clear(&res->address);
    string_clear(&res->login);

    res->activated = 0;
    integer_clear(&res->freeze);

    res->hash_type = 0;
    res->crypto_type = 0;
    res->crypto_base = 0;
}
int8_t account_cmp(const struct account_st *obj1, const struct account_st *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(&obj1->address, &obj2->address)) return CMP_NEQ;
    return CMP_EQ;
}

// Data Methods
void account_data_init(struct account_st *res) {
    if (res == NULL) return;
    string_data_init(&res->address);
    string_data_init(&res->login);

    res->activated = 0;
    integer_data_init(&res->freeze);

    res->hash_type = 0;
    res->crypto_type = 0;
    res->crypto_base = 0;
}
void account_data_free(struct account_st *res) {
    if (res == NULL) return;
    string_data_free(&res->address);
    string_data_free(&res->login);

    integer_data_free(&res->freeze);
}

// Cmp Methods
int8_t account_is_null(const struct account_st *a) {
    return (int8_t) (a == NULL || string_is_null(&a->address));
}

// TLV Methods
int8_t account_set_tlv(struct account_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    account_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_ACCOUNT) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    size_t _activated;
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->address, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->login, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = size_set_tlv(&_activated, &_tlv_data))) goto end;
    res->activated = (int8_t) _activated;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->freeze, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = size_set_tlv(&res->hash_type, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = size_set_tlv(&res->crypto_type, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = size_set_tlv(&res->crypto_base, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void account_get_tlv(const struct account_st *account, struct string_st *res) {
    if (res == NULL) return;
    if (account == NULL) return string_clear(res);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    string_get_tlv(&account->address, res);

    string_get_tlv(&account->login, &_tlv_data);
    string_concat(res, &_tlv_data);

    size_get_tlv(account->activated, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&account->freeze, &_tlv_data);
    string_concat(res, &_tlv_data);

    size_get_tlv(account->hash_type, &_tlv_data);
    string_concat(res, &_tlv_data);

    size_get_tlv(account->crypto_type, &_tlv_data);
    string_concat(res, &_tlv_data);

    size_get_tlv(account->crypto_base, &_tlv_data);
    string_concat(res, &_tlv_data);

    tlv_set_string(res, TLV_ACCOUNT, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *account_attrib
(struct error_st *err, const struct account_st *account, const struct string_st *str) {
    struct object_st *res = object_new();
    if (res == NULL) {
        error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
        return NULL;
    }
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
        integer_set_ui(res->data, account->activated);
    }
    else if (str->size == 6 && memcmp(str->data, "freeze", 6) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &account->freeze);
    }
    else if (str->size == 9 && memcmp(str->data, "hash_type", 9) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set_ui(res->data, account->hash_type);
    }
    else if (str->size == 11 && memcmp(str->data, "crypto_type", 11) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set_ui(res->data, account->crypto_type);
    }
    else if (str->size == 11 && memcmp(str->data, "crypto_base", 11) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set_ui(res->data, account->crypto_base);
    }
    else {
        object_clear(res);
        error_set_msg(err, ErrorType_Math, "This Attribute does not exist");
        return NULL;
    }
    return res;
}
