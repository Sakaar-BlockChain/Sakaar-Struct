#include "basic.h"

struct object_sub active_acc_sub = {NULL, METHOD_ATTRIB &activated_accounts_attrib};
struct object_tlv active_acc_tlv = {METHOD_GET_TLV &activated_accounts_get_tlv, METHOD_SET_TLV &activated_accounts_set_tlv};
struct object_type active_acc_type = {ACTIVE_ACC_OP, &active_acc_tlv, &active_acc_sub};

// Standard operations
struct activated_accounts *activated_accounts_new() {
    struct activated_accounts *res = skr_malloc(sizeof(struct activated_accounts));
    res->addresses = list_new();
    return res;
}
void activated_accounts_free(struct activated_accounts *res) {
    if (res == NULL) return;
    list_free(res->addresses);
    skr_free(res);
}

void activated_accounts_set(struct activated_accounts *res, const struct activated_accounts *a) {
    if (res == NULL) return;
    if (a == NULL) return activated_accounts_clear(res);

    list_set(res->addresses, a->addresses);
}
void activated_accounts_copy(struct activated_accounts *res, const struct activated_accounts *a) {
    if (res == NULL) return;
    if (a == NULL) return activated_accounts_clear(res);

    list_copy(res->addresses, a->addresses);
}

void activated_accounts_clear(struct activated_accounts *res) {
    if (res == NULL) return;
    list_clear(res->addresses);
}
int activated_accounts_cmp(const struct activated_accounts *obj1, const struct activated_accounts *obj2) {
    if (obj1 == NULL || obj2 == NULL) return 2;
    return 0;
}

// TLV Methods
void activated_accounts_set_tlv(struct activated_accounts *res, const struct string_st *tlv) {
    if (res == NULL) return;
    activated_accounts_clear(res);
    if (tlv == NULL) return;
    if (tlv_get_tag(tlv->data) != TLV_ACTIVE_ACC) return;

    char *data = tlv_get_value(tlv->data);
    struct string_st *_tlv = string_new();

    tlv_get_next_tlv(data, _tlv);
    list_set_tlv_self(res->addresses, _tlv, LIST_TYPE);
    {
        struct list_st *sub_list;
        for (size_t i = 0; i < res->addresses->size; i++) {
            sub_list = res->addresses->data[i]->data;
            object_set_tlv_self(sub_list->data[0], STRING_TYPE);
            object_set_tlv_self(sub_list->data[1], INTEGER_TYPE);
        }
    }

    string_free(_tlv);
}
void activated_accounts_get_tlv(const struct activated_accounts *active_acc, struct string_st *res) {
    if (res == NULL) return;
    if (active_acc == NULL) return string_clear(res);

    list_get_tlv(active_acc->addresses, res);
    tlv_set_string(res, TLV_ACTIVE_ACC, res);
}

// Attrib Methods
struct object_st *activated_accounts_attrib
(struct object_st *err, const struct activated_accounts *active_acc, const struct string_st *str) {
    struct object_st *res = object_new();
    if (str->size == 9 && memcmp(str->data, "addresses", 9) == 0) {
        object_set_type(res, LIST_TYPE);
        list_set(res->data, active_acc->addresses);
    }
    else {
        object_free(res);
        object_set_type(err, STRING_TYPE);
        string_set_str(err->data, "This Attribute does not exist", 29);
        return NULL;
    }
    return res;
}
