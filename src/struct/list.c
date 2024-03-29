#include "struct.h"

struct object_sub list_sub = {METHOD_SUBSCRIPT &list_subscript};
struct object_tlv list_tlv = {METHOD_GET_TLV &list_get_tlv, METHOD_SET_TLV &list_set_tlv};
struct object_math_op list_math = {NULL, NULL, METHOD_MATH &list__mul, METHOD_MATH &list__add};
struct object_convert list_convert = {NULL, NULL, NULL, METHOD_CONVERT &list__str};
struct object_type list_type = {LIST_OP, &list_tlv, &list_sub,  &list_convert, &list_math};

// Standard operations
struct list_st *list_new() {
    struct list_st *res = malloc(sizeof(struct list_st));
    res->data = NULL;
    res->max_size = 0;
    res->size = 0;
    return res;
}
void list_free(struct list_st *res) {
    if (res == NULL) return;
    list_resize(res, 0);
    if (res->data != NULL) free(res->data);
    free(res);
}

void list_set(struct list_st *res, const struct list_st *a) {
    if (res == NULL) return;
    list_clear(res);
    if (a == NULL) return;
    list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i++) res->data[i] = object_copy_obj(a->data[i]);
}
void list_copy(struct list_st *res, const struct list_st *a) {
    if (res == NULL) return;
    list_clear(res);
    if (a == NULL) return;
    list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i++) {
        res->data[i] = object_new();
        object_copy(res->data[i], a->data[i]);
    }
}

void list_mark(struct list_st *res) {
    if (res == NULL) return;
    for (size_t i = 0, size = res->size; i < size; i++)
        object_mark(res->data[i]);
}
void list_unmark(struct list_st *res) {
    if (res == NULL) return;
    for (size_t i = 0, size = res->size; i < size; i++)
        object_unmark(res->data[i]);
}

void list_clear(struct list_st *res) {
    if (res == NULL) return;
    list_resize(res, 0);
}
int8_t list_cmp(const struct list_st *obj1, const struct list_st *obj2) {
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    if (obj1->size > obj2->size) return CMP_GRET;
    if (obj1->size < obj2->size) return CMP_LESS;
    int8_t res_cmp_sub;
    for (size_t i = 0, size = obj1->size; i < size; i++) {
        res_cmp_sub = object_cmp(obj1->data[i], obj2->data[i]);
        if (res_cmp_sub != CMP_EQ) return res_cmp_sub;
    }
    return CMP_EQ;
}

// Cmp Methods
int8_t list_is_null(const struct list_st *res) {
    return (int8_t) (res == NULL || res->size == 0);
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
    if (res->data != NULL) free(res->data);
}

// Class Methods
void list_resize(struct list_st *res, size_t size) {
    if (res->data == NULL && size) {
        res->max_size = size;
        res->data = malloc(sizeof(struct object_st *) * size);
        for (size_t i = 0; i < size; i++) res->data[i] = NULL;
    } else if (res->max_size < size) {
        res->data = realloc(res->data, sizeof(struct object_st *) * size * 2);
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->data[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->size > size) {
        for (size_t i = size, l = res->size; i < l; i++) {
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
    for (size_t i = 0, size = a->size; i < size; i++) {
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
    if (res == NULL || res->size == 0) return NULL;

    struct object_st *ret = res->data[--res->size];
    res->data[res->size] = NULL;
    return ret;
}

// TLV Methods
int8_t list_set_tlv(struct list_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    list_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_LIST) return ERR_TLV_TAG;

    struct object_st *obj = NULL, *obj_ = NULL;
    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    result = tlv_get_value(tlv, &_tlv);

    for (; _tlv.size && result == 0;) {
        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        obj = object_new();
        obj_ = obj;
        if ((result = object_set_tlv(obj, &_tlv_data))) break;
        while(obj_ != NULL && obj_->type == OBJECT_TYPE) obj_ = obj_->data;
        list_append(res, obj_);
        object_free(obj);
    }

    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void list_get_tlv(const struct list_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    string_clear(tlv);
    if (res == NULL) return;

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    for (size_t i = 0, size = res->size; i < size; i++) {
        object_get_tlv(res->data[i], &_tlv_data);
        string_concat(tlv, &_tlv_data);
    }
    tlv_set_string(tlv, TLV_LIST, tlv);
    string_data_free(&_tlv_data);
}

// Convert Methods
struct object_st *list_subscript(struct error_st *err, struct list_st *list, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj == NULL) {
        error_set_msg(err, ErrorType_Math, "Can not make operation with object None");
        return NULL;
    }
    if (obj->type != INTEGER_TYPE) {
        struct object_st *temp = object_new();
        if (temp == NULL) {
            error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
            return NULL;
        }
        object__int(temp, err, obj);
        struct object_st *res = NULL;

        if (err == NULL || !err->present) {
            res = list->data[integer_get_ui(temp->data) % list->size];
        }
        object_free(temp);
        return res;
    }
    return list->data[integer_get_ui(obj->data) % list->size];
}

// Convert Methods
void list__str(struct object_st *res, struct error_st *err, const struct list_st *obj) {
    object_set_type(res, STRING_TYPE);
    string_set_str(res->data, "[", 1);
    struct object_st *temp = object_new();
    if (temp == NULL) {
        return error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
    }
    for(size_t i = 0, size = obj->size; i < size; i++) {
        object__str(temp, err, obj->data[i]);
        if (err != NULL && err->present) {
            object_free(temp);
            return;
        }
        string_concat(res->data, temp->data);
        if (i + 1 < obj->size) {
            string_set_str(temp->data, ", ", 2);
            string_concat(res->data, temp->data);
        }
    }
    string_set_str(temp->data, "]", 1);
    string_concat(res->data, temp->data);
    object_free(temp);
}

// Math Methods
void list__mul(struct object_st *res, struct error_st *err, const struct list_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 == NULL) return error_set_msg(err, ErrorType_Math, "Can not make operation with object None");
    if (obj2->type != INTEGER_TYPE) {
        struct object_st *temp = object_new();
        if (temp == NULL) {
            return error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
        }
        object__int(temp, err, obj2);

        if (err == NULL || !err->present) {
            object_set_type(res, LIST_TYPE);
            unsigned int count = integer_get_ui(temp->data);
            for (unsigned int i = 0; i < count; i++)
                list_concat(res->data, obj1);
        }
        return object_free(temp);
    }
    object_set_type(res, LIST_TYPE);
    unsigned int count = integer_get_ui(obj2->data);
    for (unsigned int i = 0; i < count; i++)
        list_concat(res->data, obj1);
}
void list__add(struct object_st *res, struct error_st *err, const struct list_st *obj1, struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 == NULL) return error_set_msg(err, ErrorType_Math, "Can not make operation with object None");

    object_set_type(res, LIST_TYPE);
    list_set(res->data, obj1);
    if (obj2->type == LIST_TYPE) list_concat(res->data, obj2->data);
    else list_append(res->data, obj2);
}
