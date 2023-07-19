#include "basic.h"

struct object_sub transaction_list_sub = {METHOD_SUBSCRIPT &transaction_list_subscript};
struct object_tlv transaction_list_tlv = {METHOD_GET_TLV &transaction_list_get_tlv, METHOD_SET_TLV &transaction_list_set_tlv};
struct object_type transaction_list_type = {TRANS_LIST_OP, &transaction_list_tlv, &transaction_list_sub};

// Standard operations
struct transaction_list_st *transaction_list_new() {
    struct transaction_list_st *res = malloc(sizeof(struct transaction_list_st));
    res->transactions = NULL;
    res->max_size = 0;
    res->size = 0;
    return res;
}
void transaction_list_free(struct transaction_list_st *res) {
    if (res == NULL) return;
    transaction_list_resize(res, 0);
    if (res->transactions != NULL) free(res->transactions);
    free(res);
}

void transaction_list_set(struct transaction_list_st *res, const struct transaction_list_st *a) {
    if (res == NULL) return;
    if (a == NULL) return transaction_list_clear(res);

    transaction_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) transaction_set(res->transactions[i], a->transactions[i]);
}
void transaction_list_copy(struct transaction_list_st *res, const struct transaction_list_st *a) {
    if (res == NULL) return;
    if (a == NULL) return transaction_list_clear(res);

    transaction_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) transaction_copy(res->transactions[i], a->transactions[i]);
}

void transaction_list_clear(struct transaction_list_st *res) {
    if (res == NULL) return;
    transaction_list_resize(res, 0);
}
int transaction_list_cmp(const struct transaction_list_st *obj1, const struct transaction_list_st *obj2) {
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    if (obj1->size > obj2->size) return CMP_GRET;
    if (obj1->size < obj2->size) return CMP_LESS;
    int res_cmp_sub;
    for (size_t i = 0, size = obj1->size; i < size; i++) {
        res_cmp_sub = transaction_cmp(obj1->transactions[i], obj2->transactions[i]);
        if (res_cmp_sub != CMP_EQ) return res_cmp_sub;
    }
    return CMP_EQ;
}

// Data Methods
void transaction_list_data_init(struct transaction_list_st *res) {
    if (res == NULL) return;
    res->transactions = NULL;
    res->max_size = 0;
    res->size = 0;
}
void transaction_list_data_free(struct transaction_list_st *res) {
    if (res == NULL) return;
    transaction_list_resize(res, 0);
    if (res->transactions != NULL) free(res->transactions);
}

// Cmp Methods
int transaction_list_is_null(const struct transaction_list_st *res) {
    return (res == NULL || res->size == 0);
}
void transaction_list_resize(struct transaction_list_st *res, size_t size) {
    if (res->transactions == NULL && size != 0) {
        res->max_size = size;
        res->transactions = malloc(sizeof(struct string_st *) * size);
        for (size_t i = 0; i < size; i++) res->transactions[i] = NULL;
    } else if (res->max_size < size) {
        res->transactions = realloc(res->transactions, sizeof(struct string_st *) * size * 2);
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->transactions[i] = NULL;
        res->max_size = size * 2;
    }
    for (size_t i = size, l = res->size; i < l; i++) {
        if (res->transactions[i] != NULL) transaction_free(res->transactions[i]);
        res->transactions[i] = NULL;
    }
    for (size_t i = res->size, l = size; i < l; i++) {
        if (res->transactions[i] == NULL) res->transactions[i] = transaction_new();
    }
    res->size = size;
}

// TLV Methods
int transaction_list_set_tlv(struct transaction_list_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    transaction_list_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_TRANS_LIST) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    result = tlv_get_value(tlv, &_tlv);

    for (size_t pos; _tlv.size && result == 0;) {
        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        pos = res->size;
        transaction_list_resize(res, pos + 1);
        result = transaction_set_tlv(res->transactions[pos], &_tlv_data);
    }

    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void transaction_list_get_tlv(const struct transaction_list_st *transaction_list, struct string_st *res) {
    if (res == NULL) return;
    string_clear(res);
    if (transaction_list == NULL) return;

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    for (size_t i = 0, size = transaction_list->size; i < size; i++)  {
        transaction_get_tlv(transaction_list->transactions[i], &_tlv_data);
        string_concat(res, &_tlv_data);
    }
    tlv_set_string(res, TLV_TRANS_LIST, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *transaction_list_subscript(struct error_st *err, struct transaction_list_st *list, const struct object_st *obj) {
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
            object_set_type(res, TRANSACTION_TYPE);
            transaction_set(res->data, list->transactions[integer_get_ui(temp->data) % list->size]);
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
    transaction_set(res->data, list->transactions[integer_get_ui(obj->data) % list->size]);
    return res;
}
