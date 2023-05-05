#include "basic.h"

struct object_sub wallet_smart_sub = {NULL, METHOD_ATTRIB &wallet_smart_attrib};
struct object_tlv wallet_smart_tlv = {METHOD_GET_TLV &wallet_smart_get_tlv, METHOD_SET_TLV &wallet_smart_set_tlv};
struct object_type wallet_smart_type = {WALLET_SMART_OP, &wallet_smart_tlv, &wallet_smart_sub};

// Standard operations
struct wallet_smart *wallet_smart_new() {
    struct wallet_smart *res = skr_malloc(sizeof(struct wallet_smart));
    string_data_init(&res->address);
    string_data_init(&res->currency);
    string_data_init(&res->private_key);
    string_data_init(&res->smart_private);
    string_data_init(&res->smart_contract);
    string_data_init(&res->name);
    string_data_init(&res->owner);

    integer_data_init(&res->freeze);
    return res;
}
void wallet_smart_free(struct wallet_smart *res) {
    if (res == NULL) return;
    string_data_free(&res->address);
    string_data_free(&res->currency);
    string_data_free(&res->private_key);
    string_data_free(&res->smart_private);
    string_data_free(&res->smart_contract);
    string_data_free(&res->name);
    string_data_free(&res->owner);

    integer_data_free(&res->freeze);
    skr_free(res);
}

void wallet_smart_set(struct wallet_smart *res, const struct wallet_smart *a) {
    if (res == NULL) return;
    if (a == NULL) return wallet_smart_clear(res);

    string_set(&res->address, &a->address);
    string_set(&res->currency, &a->currency);
    string_set(&res->private_key, &a->private_key);
    string_set(&res->smart_private, &a->smart_private);
    string_set(&res->smart_contract, &a->smart_contract);
    string_set(&res->name, &a->name);
    string_set(&res->owner, &a->owner);

    integer_set(&res->freeze, &a->freeze);
}
void wallet_smart_copy(struct wallet_smart *res, const struct wallet_smart *a) {
    if (res == NULL) return;
    if (a == NULL) return wallet_smart_clear(res);

    string_copy(&res->address, &a->address);
    string_copy(&res->currency, &a->currency);
    string_copy(&res->private_key, &a->private_key);
    string_copy(&res->smart_private, &a->smart_private);
    string_copy(&res->smart_contract, &a->smart_contract);
    string_copy(&res->name, &a->name);
    string_copy(&res->owner, &a->owner);

    integer_copy(&res->freeze, &a->freeze);
}

void wallet_smart_clear(struct wallet_smart *res) {
    if (res == NULL) return;
    string_clear(&res->address);
    string_clear(&res->currency);
    string_clear(&res->private_key);
    string_clear(&res->smart_private);
    string_clear(&res->smart_contract);
    string_clear(&res->name);
    string_clear(&res->owner);

    integer_clear(&res->freeze);
}
int wallet_smart_cmp(const struct wallet_smart *obj1, const struct wallet_smart *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(&obj1->address, &obj2->address) != 0 || string_cmp(&obj1->currency, &obj2->currency) != 0) return 2;
    return 0;
}

// TLV Methods
int wallet_smart_set_tlv(struct wallet_smart *res, const struct string_st *tlv) {
    if (res == NULL) return 0;
    wallet_smart_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_WALLET_SMART) return ERR_TLV_TAG;

    struct string_st _tlv = {NULL, 0, 0}, _tlv_data  = {NULL, 0, 0};
    if ((result = tlv_get_value(tlv, &_tlv)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = string_set_tlv(&res->address, &_tlv_data)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = string_set_tlv(&res->currency, &_tlv_data)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = string_set_tlv(&res->private_key, &_tlv_data)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = string_set_tlv(&res->smart_private, &_tlv_data)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = string_set_tlv(&res->smart_contract, &_tlv_data)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = string_set_tlv(&res->name, &_tlv_data)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = string_set_tlv(&res->owner, &_tlv_data)) != 0) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data)) != 0) goto end;
    if ((result = integer_set_tlv(&res->freeze, &_tlv_data)) != 0) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void wallet_smart_get_tlv(const struct wallet_smart *smart, struct string_st *res) {
    if (res == NULL) return;
    if (smart == NULL) return string_clear(res);

    struct string_st _tlv_data = {NULL, 0, 0};
    string_get_tlv(&smart->address, res);

    string_get_tlv(&smart->currency, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&smart->private_key, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&smart->smart_private, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&smart->smart_contract, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&smart->name, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&smart->owner, &_tlv_data);
    string_concat(res, &_tlv_data);

    integer_get_tlv(&smart->freeze, &_tlv_data);
    string_concat(res, &_tlv_data);

    tlv_set_string(res, TLV_WALLET_SMART, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *wallet_smart_attrib
(struct object_st *err, const struct wallet_smart *smart, const struct string_st *str) {
    struct object_st *res = object_new();
    if (str->size == 7 && memcmp(str->data, "address", 7) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &smart->address);
    }
    else if (str->size == 8 && memcmp(str->data, "currency", 8) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &smart->currency);
    }
    else if (str->size == 11 && memcmp(str->data, "private_key", 11) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &smart->private_key);
    }
    else if (str->size == 13 && memcmp(str->data, "smart_private", 13) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &smart->smart_private);
    }
    else if (str->size == 14 && memcmp(str->data, "smart_contract", 14) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &smart->smart_contract);
    }
    else if (str->size == 4 && memcmp(str->data, "name", 4) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &smart->name);
    }
    else if (str->size == 5 && memcmp(str->data, "owner", 5) == 0) {
        object_set_type(res, STRING_TYPE);
        string_set(res->data, &smart->owner);
    }
    else if (str->size == 6 && memcmp(str->data, "freeze", 6) == 0) {
        object_set_type(res, INTEGER_TYPE);
        integer_set(res->data, &smart->freeze);
    }
    else {
        object_free(res);
        object_set_type(err, STRING_TYPE);
        string_set_str(err->data, "This Attribute does not exist", 29);
        return NULL;
    }
    return res;
}
