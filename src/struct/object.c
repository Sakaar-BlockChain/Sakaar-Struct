#include "struct.h"

struct object_type object_type = {OBJECT_OP, METHOD_GET_TLV &object_get_tlv, METHOD_SET_TLV &object_set_tlv};




struct object_st *object_new() {
    struct object_st *res = skr_malloc(OBJECT_SIZE);
    res->type = NULL;
    res->data = NULL;
    res->counter = 1;

    res->name = NULL;
    res->dir = NULL;
    res->class = NULL;
    res->function = NULL;
    return res;
}
void object_set(struct object_st *res, const struct object_st *a) {
    if (res == NULL || a == NULL) return;
    object_set_type(res, a->type);
    if (res->type != NULL && res->type->self._set != NULL) res->type->self._set(res->data, (a)->data);
}
void object_clear(struct object_st *res) {
    if (res == NULL) return;
    object_set_type(res, NONE_TYPE);
}
void object_free(struct object_st *res) {
    if (res == NULL || --res->counter > 0) return;
    if (res->data != NULL) {
        if (res->type != NULL && res->type->self._free != NULL)
            res->type->self._free(res->data);
        res->data = NULL;
    }
    if (res->class != NULL) object_free(res->class);
    if (res->name != NULL) string_free(res->name);
    if (res->dir != NULL) list_free(res->dir);
    skr_free(res);
}
int object_cmp(const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;

    if (obj1 == obj2) return 0;
    if (obj1 == NULL || obj2 == NULL) return 2;
    if (obj1->type != obj2->type) return 2;
    if (obj1->type != NULL && obj1->type->self._cmp != NULL) return obj1->type->self._cmp(obj1->data, obj2->data);
    return 2;
}


struct object_st *object_copy(struct object_st *res) {
    if (res == NULL) return NULL;
    res->counter++;
    return res;
}
void object_set_type(struct object_st *res, struct object_type *type) {
    if (res == NULL) return;
    if (res->type == type) return;
    if (res->type != NULL) {
        if (res->data != NULL) {
            if (res->type != NULL && res->type->self._free != NULL)
                res->type->self._free(res->data);
            res->data = NULL;
        }
    }
    if (res->class != NULL) object_free(res->class);
    if (res->name != NULL) string_free(res->name);
    if (res->dir != NULL) list_free(res->dir);
    res->type = type;
    res->name = NULL;
    res->dir = NULL;
    res->class = NULL;
    res->function = NULL;
    if (res->type != NULL && res->type->self._new != NULL) res->data = res->type->self._new();
}
void object_set_ptr(struct object_st *res, struct object_st *data) {
    if (res == NULL || data == NULL) return;
    if (res->type != NULL) {
        if (res->data != NULL) {
            if (res->type != NULL && res->type->self._free != NULL)
                res->type->self._free(res->data);
            res->data = NULL;
        }
    }
    if (res->class != NULL) object_free(res->class);
    if (res->name != NULL) string_free(res->name);
    if (res->dir != NULL) list_free(res->dir);
    res->type = OBJECT_TYPE;
    res->name = NULL;
    res->dir = NULL;
    res->class = NULL;
    res->function = NULL;
    res->data = object_copy(data);
}

// TLV methods
void object_set_tlv(struct object_st *res, const struct string_st *tlv){
    object_set_type(res, TLV_TYPE);
    string_set(res->data, tlv);
}
void object_get_tlv(const struct object_st *res, struct string_st *tlv){
    while(res != NULL && res->type == OBJECT_TYPE) res = res->data;
    if(res == NULL) return;
    string_clear(tlv);
    if(res == NULL) return;
    if (res->type != NULL && res->type->_get_tlv != NULL) res->type->_get_tlv(res->data, tlv);
}
void object_set_tlv_self(struct object_st *res, struct object_type *type){
    if(res->type != TLV_TYPE) return object_set_type(res, type);

    struct string_st *tlv = string_new();
    string_set(tlv, res->data);

    object_set_type(res, type);
    if (res->type != NULL && res->type->_set_tlv != NULL) res->type->_set_tlv(res->data, tlv);
    string_free(tlv);
}


