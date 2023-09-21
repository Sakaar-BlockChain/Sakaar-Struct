#include "basic.h"

struct object_sub pre_transaction_list_sub = {METHOD_SUBSCRIPT &pre_transaction_list_subscript};
struct object_type pre_transaction_list_type = {PRE_TRANS_LIST_OP, NULL, &pre_transaction_list_sub};

// Standard operations
struct pre_transaction_list_st *pre_transaction_list_new() {
    struct pre_transaction_list_st *res = malloc(sizeof(struct pre_transaction_list_st));
    res->transactions = NULL;
    res->max_size = 0;
    res->size = 0;
    return res;
}
void pre_transaction_list_free(struct pre_transaction_list_st *res) {
    if (res == NULL) return;
    pre_transaction_list_resize(res, 0);
    if (res->transactions != NULL) free(res->transactions);
    free(res);
}

void pre_transaction_list_set(struct pre_transaction_list_st *res, const struct pre_transaction_list_st *a) {
    if (res == NULL) return;
    if (a == NULL) return pre_transaction_list_clear(res);

    pre_transaction_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) pre_transaction_set(res->transactions[i], a->transactions[i]);
}
void pre_transaction_list_copy(struct pre_transaction_list_st *res, const struct pre_transaction_list_st *a) {
    if (res == NULL) return;
    if (a == NULL) return pre_transaction_list_clear(res);

    pre_transaction_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) pre_transaction_copy(res->transactions[i], a->transactions[i]);
}

void pre_transaction_list_clear(struct pre_transaction_list_st *res) {
    if (res == NULL) return;
    pre_transaction_list_resize(res, 0);
}
int8_t pre_transaction_list_cmp(const struct pre_transaction_list_st *obj1, const struct pre_transaction_list_st *obj2) {
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    if (obj1->size > obj2->size) return CMP_GRET;
    if (obj1->size < obj2->size) return CMP_LESS;
    int8_t res_cmp_sub;
    for (size_t i = 0, size = obj1->size; i < size; i++) {
        res_cmp_sub = pre_transaction_cmp(obj1->transactions[i], obj2->transactions[i]);
        if (res_cmp_sub != CMP_EQ) return res_cmp_sub;
    }
    return CMP_EQ;
}

// Data Methods
void pre_transaction_list_data_init(struct pre_transaction_list_st *res) {
    if (res == NULL) return;
    res->transactions = NULL;
    res->max_size = 0;
    res->size = 0;
}
void pre_transaction_list_data_free(struct pre_transaction_list_st *res) {
    if (res == NULL) return;
    pre_transaction_list_resize(res, 0);
    if (res->transactions != NULL) free(res->transactions);
}

// Cmp Methods
int8_t pre_transaction_list_is_null(const struct pre_transaction_list_st *res) {
    return (int8_t) (res == NULL || res->size == 0);
}
void pre_transaction_list_resize(struct pre_transaction_list_st *res, size_t size) {
    if (res->transactions == NULL && size != 0) {
        res->max_size = size;
        res->transactions = malloc(sizeof(struct transaction_st *) * size);
        for (size_t i = 0; i < size; i++) res->transactions[i] = NULL;
    } else if (res->max_size < size) {
        res->transactions = realloc(res->transactions, sizeof(struct transaction_st *) * size * 2);
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->transactions[i] = NULL;
        res->max_size = size * 2;
    }
    for (size_t i = size, l = res->size; i < l; i++) {
        if (res->transactions[i] != NULL) pre_transaction_free(res->transactions[i]);
        res->transactions[i] = NULL;
    }
    for (size_t i = res->size, l = size; i < l; i++) {
        if (res->transactions[i] == NULL) res->transactions[i] = pre_transaction_new();
    }
    res->size = size;
}

// Attrib Methods
struct object_st *pre_transaction_list_subscript(struct error_st *err, struct pre_transaction_list_st *list, const struct object_st *obj) {
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
            object_set_type(res, PRE_TRANSACTION_TYPE);
            pre_transaction_set(res->data, list->transactions[integer_get_ui(temp->data) % list->size]);
        }
        object_free(temp);
        return res;
    }
    res = object_new();
    if (res == NULL) {
        error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
        return NULL;
    }
    object_set_type(res, PRE_TRANSACTION_TYPE);
    pre_transaction_set(res->data, list->transactions[integer_get_ui(obj->data) % list->size]);
    return res;
}
