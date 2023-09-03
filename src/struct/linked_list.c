#include "struct.h"

struct object_sub linked_list_sub = {METHOD_SUBSCRIPT &linked_list_subscript};
struct object_tlv linked_list_tlv = {METHOD_GET_TLV &linked_list_get_tlv, METHOD_SET_TLV &linked_list_set_tlv};
struct object_math_op linked_list_math = {NULL, NULL, METHOD_MATH &linked_list__mul, METHOD_MATH &linked_list__add};
struct object_convert linked_list_convert = {NULL, NULL, NULL, METHOD_CONVERT &linked_list__str};
struct object_type linked_list_type = {LINKED_LIST_OP, &linked_list_tlv, &linked_list_sub,  &linked_list_convert, &linked_list_math};

struct linked_list_elm {
    struct object_st *data;

    struct linked_list_elm *next;
    struct linked_list_elm *priv;
};

struct linked_list_elm *linked_list_elm_new() {
    struct linked_list_elm *res = malloc(sizeof(struct linked_list_elm));
    res->data = NULL;

    res->next = NULL;
    res->priv = NULL;
    return res;
}
void linked_list_elm_free(struct linked_list_elm *res) {
    if (res == NULL) return;
    if (res->data != NULL) object_free(res->data);
    free(res);
}



// Standard operations
struct linked_list_st *linked_list_new() {
    struct linked_list_st *res = malloc(sizeof(struct linked_list_st));
    res->size = 0;

    res->front = NULL;
    res->back = NULL;
    return res;
}
void linked_list_free(struct linked_list_st *res) {
    if (res == NULL) return;
    struct linked_list_elm *elm = res->front;
    struct linked_list_elm *next = NULL;
    while (elm != NULL) {
        next = elm->next;
        linked_list_elm_free(elm);
        elm = next;
    }
    free(res);
}

void linked_list_set(struct linked_list_st *res, const struct linked_list_st *a) {
    if (res == NULL) return;
    linked_list_clear(res);
    if (a == NULL) return;

    struct linked_list_elm *elm = a->front;
    while(elm != NULL) {
        linked_list_push_back(res, elm->data);
        elm = elm->next;
    }
}
void linked_list_copy(struct linked_list_st *res, const struct linked_list_st *a) {
    if (res == NULL) return;
    linked_list_clear(res);
    if (a == NULL) return;

    struct linked_list_elm *elm = a->front;
    while(elm != NULL) {
        linked_list_new_back(res, NONE_TYPE);
        object_copy(res->back->data, elm->data);
        elm = elm->next;
    }
}

void linked_list_mark(struct linked_list_st *res) {
    if (res == NULL) return;
    struct linked_list_elm *elm = res->front;
    while (elm != NULL) {
        object_mark(elm->data);
        elm = elm->next;
    }
}
void linked_list_unmark(struct linked_list_st *res) {
    if (res == NULL) return;
    struct linked_list_elm *elm = res->front;
    while (elm != NULL) {
        object_unmark(elm->data);
        elm = elm->next;
    }
}

void linked_list_clear(struct linked_list_st *res) {
    if (res == NULL) return;

    struct linked_list_elm *elm = res->front;
    struct linked_list_elm *next = NULL;
    while (elm != NULL) {
        next = elm->next;
        linked_list_elm_free(elm);
        elm = next;
    }
    res->size = 0;

    res->front = NULL;
    res->back = NULL;
}
int8_t linked_list_cmp(const struct linked_list_st *obj1, const struct linked_list_st *obj2) {
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    if (obj1->size > obj2->size) return CMP_GRET;
    if (obj1->size < obj2->size) return CMP_LESS;
    int8_t res_cmp_sub;
    struct linked_list_elm *elm1 = obj1->front;
    struct linked_list_elm *elm2 = obj2->front;
    while (elm1 != NULL && elm2 != NULL) {
        res_cmp_sub = object_cmp(elm1->data, elm2->data);
        if (res_cmp_sub != CMP_EQ) return res_cmp_sub;
        elm1 = elm1->next;
        elm2 = elm2->next;
    }
    return CMP_EQ;
}

// Cmp Methods
int8_t linked_list_is_null(const struct linked_list_st *res) {
    return (int8_t) (res == NULL || res->size == 0);
}

// Data Methods
void linked_list_data_init(struct linked_list_st *res) {
    if (res == NULL) return;
    res->size = 0;

    res->front = NULL;
    res->back = NULL;
}
void linked_list_data_free(struct linked_list_st *res) {
    if (res == NULL) return;
    struct linked_list_elm *elm = res->front;
    struct linked_list_elm *next = NULL;
    while (elm != NULL) {
        next = elm->next;
        linked_list_elm_free(elm);
        elm = next;
    }
}

// Class Methods
void linked_list_push_front(struct linked_list_st *res, struct object_st *obj) {
    if (res == NULL || obj == NULL) return;

    struct linked_list_elm *elm = linked_list_elm_new();
    res->size++;
    if (res->size == 1) {
        res->back = res->front = elm;
    } else {
        res->front->priv = elm;
        elm->next = res->front;
        res->front = elm;
    }

    elm->data = object_copy_obj(obj);
}
void linked_list_push_back(struct linked_list_st *res, struct object_st *obj) {
    if (res == NULL || obj == NULL) return;

    struct linked_list_elm *elm = linked_list_elm_new();
    res->size ++;
    if (res->size == 1) {
        res->back = res->front = elm;
    } else {
        res->back->next = elm;
        elm->priv = res->back;
        res->back = elm;
    }

    elm->data = object_copy_obj(obj);
}
void linked_list_pop_front(struct linked_list_st *res) {
    if (res == NULL || res->size == 0) return;

    struct linked_list_elm *elm = res->front;
    res->size --;
    if (res->size == 0) {
        res->back = res->front = NULL;
    } else {
        res->front = elm->next;
        res->front->priv = NULL;
        elm->next = NULL;
    }

    linked_list_elm_free(elm);
}
void linked_list_pop_back(struct linked_list_st *res) {
    if (res == NULL || res->size == 0) return;

    struct linked_list_elm *elm = res->back;
    res->size --;
    if (res->size == 0) {
        res->back = res->front = NULL;
    } else {
        res->back = elm->priv;
        res->back->next = NULL;
        elm->priv = NULL;
    }

    linked_list_elm_free(elm);
}
void linked_list_new_front(struct linked_list_st *res, struct object_type *type) {
    if (res == NULL) return;

    struct linked_list_elm *elm = linked_list_elm_new();
    res->size++;
    if (res->size == 1) {
        res->back = res->front = elm;
    } else {
        res->front->priv = elm;
        elm->next = res->front;
        res->front = elm;
    }

    object_set_type(elm->data = object_new(), type);
}
void linked_list_new_back(struct linked_list_st *res, struct object_type *type)  {
    if (res == NULL) return;

    struct linked_list_elm *elm = linked_list_elm_new();
    res->size ++;
    if (res->size == 1) {
        res->back = res->front = elm;
    } else {
        res->back->next = elm;
        elm->priv = res->back;
        res->back = elm;
    }

    object_set_type(elm->data = object_new(), type);
}
void linked_list_concat(struct linked_list_st *res, const struct linked_list_st *a) {
    if (res == NULL || linked_list_is_null(a)) return;

    struct linked_list_elm *elm = a->front;
    while(elm != NULL) {
        linked_list_push_back(res, elm->data);
        elm = elm->next;
    }
}
struct object_st *linked_list_front(struct linked_list_st *res) {
    if (res == NULL || res->size == 0) return NULL;
    return object_copy_obj(res->front->data);
}
struct object_st *linked_list_back(struct linked_list_st *res) {
    if (res == NULL || res->size == 0) return NULL;
    return object_copy_obj(res->back->data);
}

// TLV Methods
int8_t linked_list_set_tlv(struct linked_list_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    linked_list_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_LINKED_LIST) return ERR_TLV_TAG;

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
        linked_list_push_back(res, obj_);
        object_free(obj);
    }

    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void linked_list_get_tlv(const struct linked_list_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    string_clear(tlv);
    if (res == NULL) return;

    struct string_st _tlv_data;
    struct linked_list_elm *elm = res->front;
    string_data_init(&_tlv_data);

    while (elm != NULL) {
        object_get_tlv(elm->data, &_tlv_data);
        string_concat(tlv, &_tlv_data);

        elm = elm->next;
    }
    tlv_set_string(tlv, TLV_LINKED_LIST, tlv);
    string_data_free(&_tlv_data);
}

// Convert Methods
struct object_st *linked_list_subscript(struct error_st *err, struct linked_list_st *linked_list, const struct object_st *obj) {
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
            struct linked_list_elm *elm = linked_list->front;
            size_t pos = integer_get_ui(temp->data) % linked_list->size;
            size_t i = 0;

            while (i < pos && elm != NULL) {
                elm = elm->next;
                i ++;
            }
            res = elm->data;
        }
        object_free(temp);
        return res;
    }
    struct linked_list_elm *elm = linked_list->front;
    size_t pos = integer_get_ui(obj->data) % linked_list->size;
    size_t i = 0;

    while (i < pos && elm != NULL) {
        elm = elm->next;
        i ++;
    }
    return elm->data;
}

// Convert Methods
void linked_list__str(struct object_st *res, struct error_st *err, const struct linked_list_st *obj) {
    object_set_type(res, STRING_TYPE);
    string_set_str(res->data, "[", 1);
    struct object_st *temp = object_new();
    if (temp == NULL) {
        return error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
    }
    struct string_st _tlv_data;
    struct linked_list_elm *elm = obj->front;
    string_data_init(&_tlv_data);
    size_t i = 0;
    while (elm != NULL) {
        object__str(temp, err, elm->data);
        if (err != NULL && err->present) {
            object_free(temp);
            return;
        }
        string_concat(res->data, temp->data);
        if (++i < obj->size) {
            string_set_str(temp->data, ", ", 2);
            string_concat(res->data, temp->data);
        }
        elm = elm->next;
    }
    string_set_str(temp->data, "]", 1);
    string_concat(res->data, temp->data);
    object_free(temp);
}

// Math Methods
void linked_list__mul(struct object_st *res, struct error_st *err, const struct linked_list_st *obj1, const struct object_st *obj2) {
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
                linked_list_concat(res->data, obj1);
        }
        return object_free(temp);
    }
    object_set_type(res, LIST_TYPE);
    unsigned int count = integer_get_ui(obj2->data);
    for (unsigned int i = 0; i < count; i++)
        linked_list_concat(res->data, obj1);
}
void linked_list__add(struct object_st *res, struct error_st *err, const struct linked_list_st *obj1, struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 == NULL) return error_set_msg(err, ErrorType_Math, "Can not make operation with object None");

    object_set_type(res, LIST_TYPE);
    linked_list_set(res->data, obj1);
    if (obj2->type == LIST_TYPE) linked_list_concat(res->data, obj2->data);
    else linked_list_push_back(res->data, obj2);
}
