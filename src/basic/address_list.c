#include "basic.h"

struct object_sub address_list_sub = {METHOD_SUBSCRIPT &address_list_subscript};
struct object_tlv address_list_tlv = {METHOD_GET_TLV &address_list_get_tlv, METHOD_SET_TLV &address_list_set_tlv};
struct object_type address_list_type = {ADDRESS_LIST_OP, &address_list_tlv, &address_list_sub};

// Standard operations
struct address_list_st *address_list_new() {
    struct address_list_st *res = malloc(sizeof(struct address_list_st));
    res->addresses = NULL;
    res->max_size = 0;
    res->size = 0;
    return res;
}
void address_list_free(struct address_list_st *res) {
    if (res == NULL) return;

    address_list_resize(res, 0);
    if (res->addresses != NULL) free(res->addresses);
    free(res);
}

void address_list_set(struct address_list_st *res, const struct address_list_st *a) {
    if (res == NULL) return;
    if (a == NULL) return address_list_clear(res);

    address_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) string_set(res->addresses[i], a->addresses[i]);
}
void address_list_copy(struct address_list_st *res, const struct address_list_st *a) {
    if (res == NULL) return;
    if (a == NULL) return address_list_clear(res);

    address_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) string_set(res->addresses[i], a->addresses[i]);
}

void address_list_clear(struct address_list_st *res) {
    if (res == NULL) return;
    address_list_resize(res, 0);
}
int address_list_cmp(const struct address_list_st *obj1, const struct address_list_st *obj2) {
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    if (obj1->size > obj2->size) return CMP_GRET;
    if (obj1->size < obj2->size) return CMP_LESS;
    int res_cmp_sub;
    for (size_t i = 0, size = obj1->size; i < size; i++) {
        res_cmp_sub = string_cmp(obj1->addresses[i], obj2->addresses[i]);
        if (res_cmp_sub != CMP_EQ) return res_cmp_sub;
    }
    return CMP_EQ;
}

// Data Methods
void address_list_data_init(struct address_list_st *res) {
    if (res == NULL) return;
    res->addresses = NULL;
    res->max_size = 0;
    res->size = 0;
}
void address_list_data_free(struct address_list_st *res) {
    if (res == NULL) return;
    address_list_resize(res, 0);
    if (res->addresses != NULL) free(res->addresses);
}

// Cmp Methods
int address_list_is_null(const struct address_list_st *res) {
    return (res == NULL || res->size == 0);
}
void address_list_resize(struct address_list_st *res, size_t size) {
    if (res->addresses == NULL && size != 0) {
        res->max_size = size;
        res->addresses = malloc(sizeof(struct string_st *) * size);
        for (size_t i = 0; i < size; i++) res->addresses[i] = NULL;
    } else if (res->max_size < size) {
        res->addresses = realloc(res->addresses, sizeof(struct string_st *) * size * 2);
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->addresses[i] = NULL;
        res->max_size = size * 2;
    }
    for (size_t i = size, l = res->size; i < l; i++) {
        if (res->addresses[i] != NULL) string_free(res->addresses[i]);
        res->addresses[i] = NULL;
    }
    for (size_t i = res->size, l = size; i < l; i++) {
        if (res->addresses[i] == NULL) res->addresses[i] = string_new();
    }
    res->size = size;
}

// TLV Methods
int address_list_set_tlv(struct address_list_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    address_list_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_ADDRESS_LIST) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    result = tlv_get_value(tlv, &_tlv);

    for (size_t pos; _tlv.size && result == 0;) {
        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        pos = res->size;
        address_list_resize(res, pos + 1);
        result = string_set_tlv(res->addresses[pos], &_tlv_data);
    }

    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void address_list_get_tlv(const struct address_list_st *address_list, struct string_st *res) {
    if (res == NULL) return;
    string_clear(res);
    if (address_list == NULL) return;

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    for (size_t i = 0, size = address_list->size; i < size; i++)  {
        string_get_tlv(address_list->addresses[i], &_tlv_data);
        string_concat(res, &_tlv_data);
    }
    tlv_set_string(res, TLV_ADDRESS_LIST, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *address_list_subscript(struct error_st *err, struct address_list_st *list, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj == NULL) {
        error_set_msg(err, ErrorType_Math, "Can not make operation with object None");
        return NULL;
    }
    struct object_st *res = NULL;
    if (obj->type != INTEGER_TYPE) {
        struct object_st *temp = object_new();
        if (temp == NULL) {
            error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
            return NULL;
        }
        object__int(temp, err, obj);

        if (err == NULL || !err->present) {
            res = object_new();
            if (res == NULL) {
                error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
                return NULL;
            }
            object_set_type(res, STRING_TYPE);
            string_set(res->data, list->addresses[integer_get_ui(temp->data) % list->size]);
        }
        object_free(temp);
        return res;
    }
    res = object_new();
    if (res == NULL) {
        error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
        return NULL;
    }
    object_set_type(res, STRING_TYPE);
    string_set(res->data, list->addresses[integer_get_ui(obj->data) % list->size]);
    return res;
}
