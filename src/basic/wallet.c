#include "basic.h"

struct object_sub wallet_sub = {NULL, METHOD_ATTRIB &wallet_attrib};
struct object_tlv wallet_tlv = {METHOD_GET_TLV &wallet_get_tlv, METHOD_SET_TLV &wallet_set_tlv};
struct object_type wallet_type = {WALLET_OP, &wallet_tlv, &wallet_sub};

// Standard operations
struct wallet_st *wallet_new() {
    struct wallet_st *res = malloc(sizeof(struct wallet_st));
    wallet_data_data_init(&res->data);
    wallet_smart_data_init(&res->smart);
    return res;
}
void wallet_free(struct wallet_st *res) {
    if (res == NULL) return;
    wallet_data_data_free(&res->data);
    wallet_smart_data_free(&res->smart);
    free(res);
}

void wallet_set(struct wallet_st *res, const struct wallet_st *a) {
    if (res == NULL) return;
    if (a == NULL) return wallet_clear(res);

    wallet_data_set(&res->data, &a->data);
    wallet_smart_set(&res->smart, &a->smart);
}
void wallet_copy(struct wallet_st *res, const struct wallet_st *a) {
    if (res == NULL) return;
    if (a == NULL) return wallet_clear(res);

    wallet_data_copy(&res->data, &a->data);
    wallet_smart_copy(&res->smart, &a->smart);
}

void wallet_clear(struct wallet_st *res) {
    if (res == NULL) return;
    wallet_data_clear(&res->data);
    wallet_smart_clear(&res->smart);
}
int wallet_cmp(const struct wallet_st *obj1, const struct wallet_st *obj2) {
    if (obj1 == NULL || obj2 == NULL || wallet_data_cmp(&obj1->data, &obj2->data) || wallet_smart_cmp(&obj1->smart, &obj2->smart)) return CMP_NEQ;
    return CMP_EQ;
}

// Data Methods
void wallet__data_init(struct wallet_st *res) {
    wallet_data_data_init(&res->data);
    wallet_smart_data_init(&res->smart);
}
void wallet__data_free(struct wallet_st *res) {
    wallet_data_data_free(&res->data);
    wallet_smart_data_free(&res->smart);
}


// TLV Methods
int wallet_set_tlv(struct wallet_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    wallet_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_WALLET) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = wallet_data_set_tlv(&res->data, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = wallet_smart_set_tlv(&res->smart, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void wallet_get_tlv(const struct wallet_st *wallet, struct string_st *res) {
    if (res == NULL) return;
    if (wallet == NULL) return string_clear(res);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    wallet_data_get_tlv(&wallet->data, res);

    wallet_smart_get_tlv(&wallet->smart, &_tlv_data);
    string_concat(res, &_tlv_data);

    tlv_set_string(res, TLV_WALLET, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *wallet_attrib
(struct error_st *err, const struct wallet_st *wallet, const struct string_st *str) {
    struct object_st *res = object_new();
    if (str->size == 12 && memcmp(str->data, "data", 4) == 0) {
        object_set_type(res, WALLET_DATA_TYPE);
        wallet_data_set(res->data, &wallet->data);
    }
    else if (str->size == 10 && memcmp(str->data, "smart", 5) == 0) {
        object_set_type(res, WALLET_SMART_TYPE);
        wallet_smart_set(res->data, &wallet->smart);
    }
    else {
        object_clear(res);
        error_set_msg(err, ErrorType_Math, "This Attribute does not exist");
        return NULL;
    }
    return res;
}
