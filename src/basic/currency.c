#include "basic.h"

struct object_sub currency_sub = {NULL, METHOD_ATTRIB &currency_attrib};
struct object_tlv currency_tlv = {METHOD_GET_TLV &currency_get_tlv, METHOD_SET_TLV &currency_set_tlv};
struct object_type currency_type = {CURRENCY_OP, &currency_tlv, &currency_sub};

// Standard operations
struct currency_st *currency_new() {
    struct currency_st *res = malloc(sizeof(struct currency_st));
    string_data_init(&res->smart_contract);
    string_data_init(&res->name);
    string_data_init(&res->info);

    integer_data_init(&res->hash_type);
    integer_data_init(&res->crypto_type);
    integer_data_init(&res->crypto_base);

    integer_data_init(&res->our);
    integer_data_init(&res->product);
    return res;
}
void currency_free(struct currency_st *res) {
    if (res == NULL) return;
    string_data_free(&res->smart_contract);
    string_data_free(&res->name);
    string_data_free(&res->info);

    integer_data_free(&res->hash_type);
    integer_data_free(&res->crypto_type);
    integer_data_free(&res->crypto_base);

    integer_data_free(&res->our);
    integer_data_free(&res->product);
    free(res);
}

void currency_set(struct currency_st *res, const struct currency_st *a) {
    if (res == NULL) return;
    if (a == NULL) return currency_clear(res);

    string_set(&res->smart_contract, &a->smart_contract);
    string_set(&res->name, &a->name);
    string_set(&res->info, &a->info);

    integer_set(&res->hash_type, &a->hash_type);
    integer_set(&res->crypto_type, &a->crypto_type);
    integer_set(&res->crypto_base, &a->crypto_base);

    integer_set(&res->our, &a->our);
    integer_set(&res->product, &a->product);
}
void currency_copy(struct currency_st *res, const struct currency_st *a) {
    if (res == NULL) return;
    if (a == NULL) return currency_clear(res);

    string_copy(&res->smart_contract, &a->smart_contract);
    string_copy(&res->name, &a->name);
    string_copy(&res->info, &a->info);

    integer_copy(&res->hash_type, &a->hash_type);
    integer_copy(&res->crypto_type, &a->crypto_type);
    integer_copy(&res->crypto_base, &a->crypto_base);

    integer_copy(&res->our, &a->our);
    integer_copy(&res->product, &a->product);
}

void currency_clear(struct currency_st *res) {
    if (res == NULL) return;
    string_clear(&res->smart_contract);
    string_clear(&res->name);
    string_clear(&res->info);

    integer_clear(&res->hash_type);
    integer_clear(&res->crypto_type);
    integer_clear(&res->crypto_base);

    integer_clear(&res->our);
    integer_clear(&res->product);
}
int currency_cmp(const struct currency_st *obj1, const struct currency_st *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(&obj1->name, &obj2->name)) return CMP_NEQ;
    return CMP_EQ;
}

// Data Methods
void currency_data_init(struct currency_st *res) {
    if (res == NULL) return;
    string_data_init(&res->smart_contract);
    string_data_init(&res->name);
    string_data_init(&res->info);

    integer_data_init(&res->hash_type);
    integer_data_init(&res->crypto_type);
    integer_data_init(&res->crypto_base);

    integer_data_init(&res->our);
    integer_data_init(&res->product);
}
void currency_data_free(struct currency_st *res) {
    if (res == NULL) return;
    string_data_free(&res->smart_contract);
    string_data_free(&res->name);
    string_data_free(&res->info);

    integer_data_free(&res->hash_type);
    integer_data_free(&res->crypto_type);
    integer_data_free(&res->crypto_base);

    integer_data_free(&res->our);
    integer_data_free(&res->product);
}

// TLV Methods
int currency_set_tlv(struct currency_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    currency_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_CURRENCY) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->name, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->smart_contract, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->info, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->hash_type, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->crypto_type, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->crypto_base, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->our, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&res->product, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void currency_get_tlv(const struct currency_st *currency, struct string_st *res) {
    if (res == NULL) return;
    if (currency == NULL) return string_clear(res);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    string_get_tlv(&currency->name, res);

    string_get_tlv(&currency->smart_contract, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&currency->info, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&currency->hash_type, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&currency->crypto_type, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&currency->crypto_base, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&currency->our, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&currency->product, &_tlv_data);
    string_concat(res, &_tlv_data);

    tlv_set_string(res, TLV_CURRENCY, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *currency_attrib
(struct error_st *err, const struct currency_st *currency, const struct string_st *str) {
    struct object_st *res = object_new();
    if (res == NULL) {
        error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
        return NULL;
    }
    if (str->size == 4 && memcmp(str->data, "name", 4) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &currency->name);
    }
    else if (str->size == 14 && memcmp(str->data, "smart_contract", 14) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &currency->smart_contract);
    }
    else if (str->size == 4 && memcmp(str->data, "info", 4) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &currency->info);
    }
    else if (str->size == 9 && memcmp(str->data, "hash_type", 9) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &currency->hash_type);
    }
    else if (str->size == 11 && memcmp(str->data, "crypto_type", 11) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &currency->crypto_type);
    }
    else if (str->size == 11 && memcmp(str->data, "crypto_base", 11) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &currency->crypto_base);
    }
    else if (str->size == 3 && memcmp(str->data, "our", 3) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &currency->our);
    }
    else if (str->size == 7 && memcmp(str->data, "product", 7) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &currency->product);
    }
    else {
        object_clear(res);
        error_set_msg(err, ErrorType_Math, "This Attribute does not exist");
        return NULL;
    }
    return res;
}
