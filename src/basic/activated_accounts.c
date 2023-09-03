#include "basic.h"

struct object_sub active_acc_sub = {};
struct object_tlv active_acc_tlv = {METHOD_GET_TLV &activated_accounts_get_tlv, METHOD_SET_TLV &activated_accounts_set_tlv};
struct object_type active_acc_type = {ACTIVE_ACC_OP, &active_acc_tlv, &active_acc_sub};

// Standard operations
struct activated_accounts *activated_accounts_new() {
    struct activated_accounts *res = malloc(sizeof(struct activated_accounts));

    res->addresses = NULL;
    res->freeze = NULL;
    res->max_size = 0;
    res->size = 0;
    return res;
}
void activated_accounts_free(struct activated_accounts *res) {
    if (res == NULL) return;

    activated_accounts_resize(res, 0);
    if (res->addresses != NULL) free(res->addresses);
    if (res->freeze != NULL) free(res->freeze);
    free(res);
}

void activated_accounts_set(struct activated_accounts *res, const struct activated_accounts *a) {
    if (res == NULL) return;
    if (a == NULL) return activated_accounts_clear(res);

    activated_accounts_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) string_set(res->addresses[i], a->addresses[i]);
    for (size_t i = 0, size = a->size; i < size; i ++) integer_set(res->freeze[i], a->freeze[i]);
}
void activated_accounts_copy(struct activated_accounts *res, const struct activated_accounts *a) {
    if (res == NULL) return;
    if (a == NULL) return activated_accounts_clear(res);

    activated_accounts_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) string_copy(res->addresses[i], a->addresses[i]);
    for (size_t i = 0, size = a->size; i < size; i ++) integer_copy(res->freeze[i], a->freeze[i]);
}

void activated_accounts_clear(struct activated_accounts *res) {
    if (res == NULL) return;
    activated_accounts_resize(res, 0);
}
int8_t activated_accounts_cmp(const struct activated_accounts *obj1, const struct activated_accounts *obj2) {
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    if (obj1->size > obj2->size) return CMP_GRET;
    if (obj1->size < obj2->size) return CMP_LESS;
    int8_t res_cmp_sub;
    for (size_t i = 0, size = obj1->size; i < size; i++) {
        res_cmp_sub = string_cmp(obj1->addresses[i], obj2->addresses[i]);
        if (res_cmp_sub != CMP_EQ) return res_cmp_sub;
        res_cmp_sub = integer_cmp(obj1->freeze[i], obj2->freeze[i]);
        if (res_cmp_sub != CMP_EQ) return res_cmp_sub;
    }
    return CMP_EQ;
}

// Data Methods
void activated_accounts_data_init(struct activated_accounts *res) {
    if (res == NULL) return;
    res->addresses = NULL;
    res->freeze = NULL;
    res->max_size = 0;
    res->size = 0;
}
void activated_accounts_data_free(struct activated_accounts *res) {
    if (res == NULL) return;
    activated_accounts_resize(res, 0);
    if (res->addresses != NULL) free(res->addresses);
    if (res->freeze != NULL) free(res->freeze);
}

// Cmp Methods
int8_t activated_accounts_is_null(const struct activated_accounts *res) {
    return (int8_t) (res == NULL || res->size == 0);
}
void activated_accounts_resize(struct activated_accounts *res, size_t size) {
    if (res->addresses == NULL && size != 0) {
        res->max_size = size;
        res->addresses = malloc(sizeof(struct string_st *) * size);
        res->freeze = malloc(sizeof(struct integer_st *) * size);
        for (size_t i = 0; i < size; i++) res->addresses[i] = NULL;
        for (size_t i = 0; i < size; i++) res->freeze[i] = NULL;
    } else if (res->max_size < size) {
        res->addresses = realloc(res->addresses, sizeof(struct string_st *) * size * 2);
        res->freeze = realloc(res->freeze, sizeof(struct integer_st *) * size * 2);
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->addresses[i] = NULL;
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->freeze[i] = NULL;
        res->max_size = size * 2;
    }
    for (size_t i = size, l = res->size; i < l; i++) {
        if (res->addresses[i] != NULL) string_free(res->addresses[i]);
        res->addresses[i] = NULL;
        if (res->freeze[i] != NULL) integer_free(res->freeze[i]);
        res->freeze[i] = NULL;
    }
    for (size_t i = res->size, l = size; i < l; i++) {
        if (res->addresses[i] == NULL) res->addresses[i] = string_new();
        if (res->freeze[i] == NULL) res->freeze[i] = integer_new();
    }
    res->size = size;
}

// TLV Methods
int8_t activated_accounts_set_tlv(struct activated_accounts *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    activated_accounts_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_ACTIVE_ACC) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    result = tlv_get_value(tlv, &_tlv);

    for (size_t pos; _tlv.size && result == 0;) {
        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        pos = res->size;

        activated_accounts_resize(res, pos + 1);
        if ((result = string_set_tlv(res->addresses[pos], &_tlv_data))) break;

        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        result = integer_set_tlv(res->freeze[pos], &_tlv_data);
    }

    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void activated_accounts_get_tlv(const struct activated_accounts *active_acc, struct string_st *res) {
    if (res == NULL) return;
    string_clear(res);
    if (active_acc == NULL) return;

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    for (size_t i = 0, size = active_acc->size; i < size; i++)  {
        string_get_tlv(active_acc->addresses[i], &_tlv_data);
        string_concat(res, &_tlv_data);

        integer_get_tlv(active_acc->freeze[i], &_tlv_data);
        string_concat(res, &_tlv_data);
    }
    tlv_set_string(res, TLV_ACTIVE_ACC, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
//struct object_st *activated_accounts_attrib
//(struct error_st *err, const struct activated_accounts *active_acc, const struct string_st *str) {
//    struct object_st *res = object_new();
//    if (str->size == 9 && memcmp(str->data, "addresses", 9) == 0) {
//        object_set_type(res, LIST_TYPE);
//        list_set(res->data, &active_acc->addresses);
//    }
//    else {
//        object_clear(res);
//        error_set_msg(err, ErrorType_Math, "This Attribute does not exist");
//        return NULL;
//    }
//    return res;
//}
