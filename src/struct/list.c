#include "struct.h"

struct object_sub list_sub = {METHOD_SUBSCRIPT &list_subscript};
struct object_tlv list_tlv = {METHOD_GET_TLV &list_get_tlv, METHOD_SET_TLV &list_set_tlv};
struct object_math_op list_math = {NULL, NULL, METHOD_MATH &list__mul, METHOD_MATH &list__add};
struct object_convert list_convert = {NULL, NULL, NULL, METHOD_CONVERT &list__str};
struct object_type list_type = {LIST_OP, &list_tlv, &list_sub,  &list_convert, &list_math};

// Standard operations
struct list_st *list_new() {
    struct list_st *res = skr_malloc(sizeof(struct list_st));
    res->data = NULL;
    res->max_size = 0;
    res->size = 0;
    return res;
}
void list_free(struct list_st *res) {
    if (res == NULL) return;
    list_resize(res, 0);
    if (res->data != NULL) skr_free(res->data);
    skr_free(res);
}

void list_set(struct list_st *res, const struct list_st *a) {
    if (res == NULL) return;
    list_clear(res);
    if (a == NULL) return;
    list_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) res->data[i] = object_copy_obj(a->data[i]);
}
void list_copy(struct list_st *res, const struct list_st *a) {
    if (res == NULL) return;
    list_clear(res);
    if (a == NULL) return list_clear(res);
    list_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) {
        res->data[i] = object_new();
        object_copy(res->data[i], a->data[i]);
    }
}

void list_clear(struct list_st *res) {
    if(res == NULL) return;
    list_resize(res, 0);
}
int list_cmp(const struct list_st *obj1, const struct list_st *obj2) {
    if (obj1 == NULL || obj2 == NULL) return 2;
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

// Cmp Methods
int list_is_null(const struct list_st *res) {
    return (res == NULL || res->size == 0);
}

// Data Methods
void list_data_init(struct list_st *res) {
    if (res == NULL) return;
    res->data = NULL;
    res->max_size = 0;
    res->size = 0;
}
void list_data_free(struct list_st *res) {
    if (res == NULL) return;
    list_resize(res, 0);
    if (res->data != NULL) skr_free(res->data);
}

// Class Methods
void list_resize(struct list_st *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->max_size = size;
        res->data = skr_malloc(sizeof(struct object_st *) * size);
        for (size_t i = 0; i < size; i++) res->data[i] = NULL;
    } else if (res->max_size < size) {
        res->data = skr_realloc(res->data, sizeof(struct object_st *) * size * 2);
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
    res->data[res->size - 1] = object_copy_obj(obj);
}
void list_concat(struct list_st *res, const struct list_st *a) {
    if (res == NULL || list_is_null(a)) return;

    size_t _size = res->size;
    list_resize(res, res->size + a->size);
    for (size_t i = 0; i < a->size; i++) {
        res->data[_size + i] = object_copy_obj(a->data[i]);
    }
}
void list_add_new(struct list_st *res, struct object_type *type) {
    if (res == NULL) return;

    list_resize(res, res->size + 1);
    res->data[res->size - 1] = object_new();
    object_set_type(res->data[res->size - 1], type);
}
struct object_st *list_pop(struct list_st *res) {
    if(res == NULL || res->size == 0) return NULL;

    struct object_st *ret = res->data[--res->size];
    res->data[res->size] = NULL;
    return ret;
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

// TLV Methods
int list_set_tlv(struct list_st *res, const struct string_st *tlv) {
    if (res == NULL) return 0;
    list_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != LIST_TLV) return ERR_TLV_TAG;

    struct string_st _tlv = {NULL, 0, 0};
    result = tlv_get_value(tlv, &_tlv);

    for (; _tlv.size != 0 && result == 0;) {
        struct object_st *obj = object_new();
        object_set_type(obj, TLV_TYPE);
        result = tlv_get_next_tlv(&_tlv, obj->data);
        list_append(res, obj);
        object_free(obj);
    }

    string_data_free(&_tlv);
    return result;
}
void list_get_tlv(const struct list_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    string_clear(tlv);
    if (res == NULL) return;

    struct string_st _tlv_data = {NULL, 0, 0};
    for (size_t i = 0; i < res->size; i++) {
        object_get_tlv(res->data[i], &_tlv_data);
        string_concat(tlv, &_tlv_data);
    }
    tlv_set_string(tlv, LIST_TLV, tlv);
    string_data_free(&_tlv_data);
}
int list_set_tlv_self(struct list_st *res, const struct string_st *tlv, struct object_type *type) {
    int result = list_set_tlv(res, tlv);
    for (size_t i = 0; i < res->size && result == 0; i++)
        result = object_set_tlv_self(res->data[i], type);
    return result;
}

// Convert Methods
struct object_st *list_subscript(struct object_st *err, struct list_st *list, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    struct object_st *temp = object_new();
    object__int(temp, err, obj);
    if(err->type != NONE_TYPE) {
        object_free(temp);
        return NULL;
    }
    size_t position = integer_get_ui(temp->data) % list->size;
    object_free(temp);
    return list->data[position];
}
void list_get_subscript(struct object_st *res, struct object_st *err, const struct list_st *list, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    struct object_st *temp = object_new();
    object__int(temp, err, obj);
    if(err->type != NONE_TYPE) {
        object_free(temp);
        return;
    }
    size_t position = integer_get_ui(temp->data) % list->size;
    object_set(res, list->data[position]);
    object_free(temp);
}

// Convert Methods
void list__str(struct object_st *res, struct object_st *err, const struct list_st *obj){
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Not Done", 8);
    //TODO
}

// Math Methods
void list__mul(struct object_st *res, struct object_st *err, const struct list_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    struct object_st *temp = object_new();
    object__int(temp, err, obj2);
    if(err->type != NONE_TYPE) {
        object_free(temp);
        return;
    }
    object_set_type(res, STRING_TYPE);
    unsigned int count = integer_get_ui(temp->data);
    for (unsigned int i = 0; i < count; i++)
        list_concat(res->data, obj1);
    object_free(temp);
}
void list__add(struct object_st *res, struct object_st *err, const struct list_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 == NULL || obj2->type != LIST_TYPE) {
        object_set_type(err, STRING_TYPE);
        string_set_str(err->data, "list dont have operation add with non list type", 47);
        return;
    }
    object_set_type(res, LIST_TYPE);
    list_set(res->data, obj1);
    list_concat(res->data, obj2->data);
}
