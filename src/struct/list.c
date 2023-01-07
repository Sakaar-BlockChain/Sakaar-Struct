#include "struct.h"

struct object_type list_type = {LIST_OP, METHOD_GET_TLV &list_get_tlv, METHOD_SET_TLV &list_set_tlv};


struct list_st *list_new() {
    struct list_st *res = skr_malloc(LIST_SIZE);
    res->data = NULL;
    res->max_size = res->size = 0;
    return res;
}
void list_set(struct list_st *res, const struct list_st *a) {
    if (a == NULL) return;
    list_resize(res, 0);
    list_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) res->data[i] = object_copy(a->data[i]);
}
void list_clear(struct list_st *res) {
    list_resize(res, 0);
}
void list_free(struct list_st *res) {
    list_resize(res, 0);
    if (res->data != NULL) skr_free(res->data);
    skr_free(res);
}
int list_cmp(const struct list_st *obj1, const struct list_st *obj2) {
    if (obj1->size > obj2->size) return 1;
    if (obj1->size < obj2->size) return -1;
    int res_cmp_sub;
    for (size_t i = 0; i < obj1->size; i++) {
        res_cmp_sub = object_cmp(obj1->data[i], obj2->data[i]);
        if (res_cmp_sub == -1) return -1;
        if (res_cmp_sub == 1) return 1;
    }
    return 0;
}
int list_is_null(const struct list_st *res) {
    return (res == NULL || res->size == 0);
}


void list_resize(struct list_st *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->max_size = size;
        res->data = skr_malloc(POINTER_SIZE * size);
        for (size_t i = 0; i < size; i++) res->data[i] = NULL;
    } else if (res->max_size < size) {
        res->data = skr_realloc(res->data, POINTER_SIZE * size * 2);
        for (size_t i = res->max_size; i < size * 2; i++) res->data[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            if (res->data[i] != NULL) object_free(res->data[i]);
            res->data[i] = NULL;
        }
    }
    res->size = size;
}
void list_append(struct list_st *res, struct object_st *obj) {
    if (res == NULL || obj == NULL) return;

    list_resize(res, res->size + 1);
    res->data[res->size - 1] = object_copy(obj);
}
void list_add_new(struct list_st *res, struct object_type *type) {
    if (res == NULL) return;

    list_resize(res, res->size + 1);
    res->data[res->size - 1] = object_new();
    object_set_type(res->data[res->size - 1], type);
}
void list_remove_last(struct list_st *res) {
    if (res == NULL) return;

    list_resize(res, res->size - 1);
}
struct object_st *list_last(struct list_st *res) {
    if (res == NULL || res->size == 0) return NULL;
    return res->data[res->size - 1];
}


void list_sort_merge(size_t st1, size_t fn1, size_t st2, size_t fn2, struct object_st **data, struct object_st **temp) {
    size_t st = st1;
    size_t pos = st1;
    while (st1 < fn1 || st2 < fn2) {
        if (st1 == fn1) {
            temp[pos++] = data[st2++];
        } else if (st2 == fn2) {
            temp[pos++] = data[st1++];
        } else {
            if (object_cmp(data[st1], data[st2]) <= 0) {
                temp[pos++] = data[st1++];
            } else {
                temp[pos++] = data[st2++];
            }
        }
    }
    for (; st < fn2; st++) {
        data[st] = temp[st];
        temp[st] = NULL;
    }
}
void list_sort_split(size_t st, size_t fn, struct object_st **data, struct object_st **temp) {
    if (st + 1 >= fn) return;
    size_t mid = (st + fn) / 2;
    list_sort_split(st, mid, data, temp);
    list_sort_split(mid, fn, data, temp);
    list_sort_merge(st, mid, mid, fn, data, temp);
}
void list_sort(struct list_st *res) {
    if(res == NULL) return;

    struct list_st *temp = list_new();
    list_resize(temp, res->size);
    list_sort_split(0, res->size, res->data, temp->data);
    list_free(temp);
}

void list_set_tlv(struct list_st *res, const struct string_st *tlv) {
    if (res == NULL) return;
    list_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != LIST_TLV) return;

    char *data = tlv_get_value(tlv->data);
    char *end = data + tlv_get_size(tlv->data);

    for (; data != end;) {
        struct object_st *obj = object_new();
        object_set_type(obj, TLV_TYPE);
        data = tlv_get_next_tlv(data, obj->data);
        list_append(res, obj);
        object_free(obj);
    }
}
void list_get_tlv(const struct list_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    string_clear(tlv);
    if (res == NULL) return;

    struct string_st *temp = string_new();
    for (size_t i = 0; i < res->size; i++) {
        object_get_tlv(res->data[i], temp);
        string_concat(tlv, temp);
    }
    tlv_set_string(tlv, LIST_TLV, tlv);
    string_free(temp);
}
void list_set_tlv_self(struct list_st *res, const struct string_st *tlv, struct object_type *type) {
    if (res == NULL) return;
    list_clear(res);
    if (string_is_null(tlv)) return;
    list_set_tlv(res, tlv);
    for (size_t i = 0; i < res->size; i++)
        object_set_tlv_self(res->data[i], type);
}
