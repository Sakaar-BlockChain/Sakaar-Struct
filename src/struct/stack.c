#include "struct.h"

struct object_type stack_type = {STACK_OP, METHOD_GET_TLV &stack_get_tlv, METHOD_SET_TLV &stack_set_tlv};

struct stack_el *stack_el_new() {
    struct stack_el *res = skr_malloc(sizeof(struct stack_el));
    res->priv = NULL;
    res->data = NULL;
    return res;
}

void stack_el_free(struct stack_el *res) {
    if (res->data != NULL) object_free(res->data);
    skr_free(res);
}

struct stack_st *stack_new() {
    struct stack_st *res = skr_malloc(STACK_SIZE);
    res->top = NULL;
    res->size = 0;
    return res;
}
void stack_set(struct stack_st *res, const struct stack_st *a) {
    if (a == NULL) return;
    stack_clear(res);

    res->size = a->size;

    if(res->size == 0) return;

    struct stack_el *cur = res->top = stack_el_new();

    struct stack_el *elm = a->top;
    while(elm != NULL){
        cur->data = object_copy(elm->data);
        elm = elm->priv;
        if(elm != NULL) cur->priv = stack_el_new();
        cur = cur->priv;
    }
}

void stack_clear(struct stack_st *res) {
    struct stack_el *elm = res->top;
    struct stack_el *next = NULL;
    while (elm != NULL) {
        next = elm->priv;
        stack_el_free(elm);
        elm = next;
    }
    res->top = NULL;
    res->size = 0;
}
void stack_free(struct stack_st *res) {
    stack_clear(res);
    skr_free(res);
}
int stack_cmp(const struct stack_st *obj1, const struct stack_st *obj2) {
    if (obj1->size > obj2->size) return 1;
    if (obj1->size < obj2->size) return -1;
    int res_cmp_sub;

    struct stack_el *elm1 = obj1->top;
    struct stack_el *elm2 = obj2->top;
    while (elm1 != NULL || elm2 != NULL) {
        if (elm1 == NULL) return -1;
        if (elm2 == NULL) return 1;

        res_cmp_sub = object_cmp(elm1->data, elm1->data);
        if (res_cmp_sub == -1) return -1;
        if (res_cmp_sub == 1) return 1;

        elm1 = elm1->priv;
        elm2 = elm2->priv;
    }
    return 0;
}
int stack_is_null(const struct stack_st *res) {
    return (res == NULL || res->size == 0);
}


void stack_add(struct stack_st *res, struct object_st *obj) {
    if (res == NULL || obj == NULL) return;
    struct stack_el *elm = stack_el_new();
    elm->priv = res->top;
    res->top = elm;
    res->size++;

    elm->data = object_copy(obj);
}
void stack_add_new(struct stack_st *res, struct object_type *type) {
    if (res == NULL) return;
    struct stack_el *elm = stack_el_new();
    elm->priv = res->top;
    res->top = elm;
    res->size++;

    elm->data = object_new();
    object_set_type(elm->data, type);
}
void stack_pop(struct stack_st *res) {
    if (res == NULL) return;
    struct stack_el *elm = res->top;
    res->top = elm->priv;
    stack_el_free(elm);
    res->size--;
}


void stack_set_tlv(struct stack_st *res, const struct string_st *tlv) {
    if (res == NULL) return;
    stack_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != STACK_TLV) return;

    char *data = tlv_get_value(tlv->data);
    char *end = data + tlv_get_size(tlv->data);

    for (; data != end;) {
        struct object_st *obj = object_new();
        object_set_type(obj, TLV_TYPE);
        data = tlv_get_next_tlv(data, obj->data);
        stack_add(res, obj);
        object_free(obj);
    }
}
void stack_get_tlv(const struct stack_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    string_clear(tlv);
    if (res == NULL) return;

    struct string_st *temp = string_new();
    for (struct stack_el *elm = res->top; elm != NULL; elm = elm->priv) {
        object_get_tlv(elm->data, temp);
        string_concat(tlv, temp);
    }
    tlv_set_string(tlv, STACK_TLV, tlv);
    string_free(temp);
}
void stack_set_tlv_self(struct stack_st *res, const struct string_st *tlv, struct object_type *type) {
    if (res == NULL) return;
    stack_clear(res);
    if (string_is_null(tlv)) return;
    stack_set_tlv(res, tlv);
    for (struct stack_el *elm = res->top; elm != NULL; elm = elm->priv)
        object_set_tlv_self(elm->data, type);
}
