#include "basic.h"

struct object_sub currency_sub = {NULL, METHOD_ATTRIB &currency_attrib};
struct object_tlv currency_tlv = {METHOD_GET_TLV &currency_get_tlv, METHOD_SET_TLV &currency_set_tlv};
struct object_type currency_type = {CURRENCY_OP, &currency_tlv, &currency_sub};

// Standard operations
struct currency_st *currency_new() {
    struct currency_st *res = skr_malloc(sizeof(struct currency_st));
    res->smart_contract = string_new();
    res->name = string_new();
    res->info = string_new();

    res->hash_type = integer_new();
    res->crypto_type = integer_new();
    res->crypto_base = integer_new();

    res->our = integer_new();
    res->product = integer_new();
    return res;
}
void currency_free(struct currency_st *res) {
    if (res == NULL) return;
    string_free(res->smart_contract);
    string_free(res->name);
    string_free(res->info);

    integer_free(res->hash_type);
    integer_free(res->crypto_type);
    integer_free(res->crypto_base);

    integer_free(res->our);
    integer_free(res->product);
    skr_free(res);
}

void currency_set(struct currency_st *res, const struct currency_st *a) {
    if (res == NULL) return;
    if (a == NULL) return currency_clear(res);

    string_set(res->smart_contract, a->smart_contract);
    string_set(res->name, a->name);
    string_set(res->info, a->info);

    integer_set(res->hash_type, a->hash_type);
    integer_set(res->crypto_type, a->crypto_type);
    integer_set(res->crypto_base, a->crypto_base);

    integer_set(res->our, a->our);
    integer_set(res->product, a->product);
}
void currency_copy(struct currency_st *res, const struct currency_st *a) {
    if (res == NULL) return;
    if (a == NULL) return currency_clear(res);

    string_copy(res->smart_contract, a->smart_contract);
    string_copy(res->name, a->name);
    string_copy(res->info, a->info);

    integer_copy(res->hash_type, a->hash_type);
    integer_copy(res->crypto_type, a->crypto_type);
    integer_copy(res->crypto_base, a->crypto_base);

    integer_copy(res->our, a->our);
    integer_copy(res->product, a->product);
}

void currency_clear(struct currency_st *res) {
    if (res == NULL) return;
    string_clear(res->smart_contract);
    string_clear(res->name);
    string_clear(res->info);

    integer_clear(res->hash_type);
    integer_clear(res->crypto_type);
    integer_clear(res->crypto_base);

    integer_clear(res->our);
    integer_clear(res->product);
}
int currency_cmp(const struct currency_st *obj1, const struct currency_st *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(obj1->name, obj2->name) != 0) return 2;
    return 0;
}

// TLV Methods
void currency_set_tlv(struct currency_st *res, const struct string_st *tlv) {
    if (res == NULL) return;
    currency_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != TLV_CURRENCY) return;

    char *data = tlv_get_value(tlv->data);
    struct string_st *_tlv = string_new();

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->name, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->smart_contract, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->info, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->hash_type, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->crypto_type, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->crypto_base, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->our, _tlv);

    tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->product, _tlv);

    string_free(_tlv);
}
void currency_get_tlv(const struct currency_st *currency, struct string_st *res) {
    if (res == NULL) return;
    if (currency == NULL) return string_clear(res);

    struct string_st *tlv = string_new();
    string_get_tlv(currency->name, res);

    string_get_tlv(currency->smart_contract, tlv);
    string_concat(res, tlv);

    string_get_tlv(currency->info, tlv);
    string_concat(res, tlv);

    integer_get_tlv(currency->hash_type, tlv);
    string_concat(res, tlv);

    integer_get_tlv(currency->crypto_type, tlv);
    string_concat(res, tlv);

    integer_get_tlv(currency->crypto_base, tlv);
    string_concat(res, tlv);

    integer_get_tlv(currency->our, tlv);
    string_concat(res, tlv);

    integer_get_tlv(currency->product, tlv);
    string_concat(res, tlv);

    tlv_set_string(res, TLV_CURRENCY, res);
    string_free(tlv);
}

// Attrib Methods
struct object_st *currency_attrib
(struct object_st *err, const struct currency_st *currency, const struct string_st *str) {
    struct object_st *res = object_new();
    if (str->size == 4 && memcmp(str->data, "name", 4) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, currency->name);
    }
    else if (str->size == 14 && memcmp(str->data, "smart_contract", 14) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, currency->smart_contract);
    }
    else if (str->size == 4 && memcmp(str->data, "info", 4) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, currency->info);
    }
    else if (str->size == 9 && memcmp(str->data, "hash_type", 9) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, currency->hash_type);
    }
    else if (str->size == 11 && memcmp(str->data, "crypto_type", 11) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, currency->crypto_type);
    }
    else if (str->size == 11 && memcmp(str->data, "crypto_base", 11) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, currency->crypto_base);
    }
    else if (str->size == 3 && memcmp(str->data, "our", 3) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, currency->our);
    }
    else if (str->size == 7 && memcmp(str->data, "product", 7) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, currency->product);
    }
    else {
        object_free(res);
        object_set_type(err, STRING_TYPE);
        string_set_str(err->data, "This Attribute does not exist", 29);
        return NULL;
    }
    return res;
}
