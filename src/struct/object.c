#include "struct.h"

struct object_tlv object_tlv = {METHOD_GET_TLV &object_get_tlv, METHOD_SET_TLV &object_set_tlv};
struct object_math_op object_math = {METHOD_MATH &object__mod, METHOD_MATH &object__and, METHOD_MATH &object__mul,
                                     METHOD_MATH &object__add, METHOD_MATH &object__sub, METHOD_MATH &object__div,
                                     METHOD_MATH &object__xor, METHOD_MATH &object__or, METHOD_MATH &object__ls,
                                     METHOD_MATH &object__rs};
struct object_convert object_convert = {METHOD_CONVERT &object__bool, METHOD_CONVERT &object__int, METHOD_CONVERT &object__float, METHOD_CONVERT &object__str};
struct object_type object_type = {OBJECT_OP, &object_tlv, &object_math, &object_convert};
// Standard operations
struct object_st *object_new() {
    struct object_st *res = skr_malloc(OBJECT_SIZE);
    res->type = NULL;
    res->data = NULL;
    res->counter = 1;
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
    skr_free(res);
}
int object_cmp(const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;

    if (obj1 == obj2) return 0;
    if (obj1 == NULL || obj2 == NULL) return 2;
    if (obj1->type != obj2->type) return 2;
    if (obj1->type != NULL && obj1->type->self._cmp != NULL) return obj1->type->self._cmp(obj1->data, obj2->data);
    return 0;
}

// Class methods
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
    res->type = type;
    if (res->type != NULL && res->type->self._new != NULL) res->data = res->type->self._new();
}

// TLV method
void object_set_tlv(struct object_st *res, const struct string_st *tlv) {
    object_set_type(res, TLV_TYPE);
    string_set(res->data, tlv);
}
void object_get_tlv(const struct object_st *res, struct string_st *tlv) {
    while (res != NULL && res->type == OBJECT_TYPE) res = res->data;
    if (res == NULL || res->type == NULL) return;
    string_clear(tlv);
    if (res->type != NULL && res->type->tlv != NULL && res->type->tlv->_get_tlv != NULL) res->type->tlv->_get_tlv(res->data, tlv);
}
void object_set_tlv_self(struct object_st *res, struct object_type *type) {
    if (res->type != TLV_TYPE) return object_set_type(res, type);

    struct string_st *tlv = string_new();
    string_set(tlv, res->data);

    object_set_type(res, type);
    if (res->type != NULL && res->type->tlv != NULL && res->type->tlv->_set_tlv != NULL) res->type->tlv->_set_tlv(res->data, tlv);
    string_free(tlv);
}

// Math methods
void object__mod(struct object_st *res, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = res->data;
    if (obj1 == NULL || obj1->type == NULL) return;
    if (obj1->type->math != NULL && obj1->type->math->_mod != NULL) {
        obj1->type->math->_mod(res, obj1->data, obj2);
    }
}
void object__and(struct object_st *res, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = res->data;
    if (obj1 == NULL || obj1->type == NULL) return;
    if (obj1->type->math != NULL && obj1->type->math->_and != NULL) {
        obj1->type->math->_and(res, obj1->data, obj2);
    }
}
void object__mul(struct object_st *res, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = res->data;
    if (obj1 == NULL || obj1->type == NULL) return;
    if (obj1->type->math != NULL && obj1->type->math->_mul != NULL) {
        obj1->type->math->_mul(res, obj1->data, obj2);
    }
}
void object__add(struct object_st *res, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = res->data;
    if (obj1 == NULL || obj1->type == NULL) return;
    if (obj1->type->math != NULL && obj1->type->math->_add != NULL) {
        obj1->type->math->_add(res, obj1->data, obj2);
    }
}
void object__sub(struct object_st *res, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = res->data;
    if (obj1 == NULL || obj1->type == NULL) return;
    if (obj1->type->math != NULL && obj1->type->math->_sub != NULL) {
        obj1->type->math->_sub(res, obj1->data, obj2);
    }
}
void object__div(struct object_st *res, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = res->data;
    if (obj1 == NULL || obj1->type == NULL) return;
    if (obj1->type->math != NULL && obj1->type->math->_div != NULL) {
        obj1->type->math->_div(res, obj1->data, obj2);
    }
}
void object__xor(struct object_st *res, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = res->data;
    if (obj1 == NULL || obj1->type == NULL) return;
    if (obj1->type->math != NULL && obj1->type->math->_xor != NULL) {
        obj1->type->math->_xor(res, obj1->data, obj2);
    }
}
void object__or(struct object_st *res, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = res->data;
    if (obj1 == NULL || obj1->type == NULL) return;
    if (obj1->type->math != NULL && obj1->type->math->_or != NULL) {
        obj1->type->math->_or(res, obj1->data, obj2);
    }
}
void object__ls(struct object_st *res, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = res->data;
    if (obj1 == NULL || obj1->type == NULL) return;
    if (obj1->type->math != NULL && obj1->type->math->_ls != NULL) {
        obj1->type->math->_ls(res, obj1->data, obj2);
    }
}
void object__rs(struct object_st *res, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = res->data;
    if (obj1 == NULL || obj1->type == NULL) return;
    if (obj1->type->math != NULL && obj1->type->math->_rs != NULL) {
        obj1->type->math->_rs(res, obj1->data, obj2);
    }
}
void object__neg(struct object_st *res, const struct object_st *obj1){
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = res->data;
    if (obj1 == NULL || obj1->type == NULL) return;
    if (obj1->type->math != NULL && obj1->type->math->_ne != NULL) {
        obj1->type->math->_ne(res, obj1->data);
    }
}

// Convert methods
void object__bool(struct object_st *res, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = res->data;
    if (obj == NULL || obj->type == NULL) {
        object_set_type(res, INTEGER_TYPE);
        integer_set_ui(res->data, 0);
        return;
    }
    if (obj->type->convert != NULL && obj->type->convert->_bool != NULL) {
        obj->type->convert->_bool(res, obj->data);
    }else{
        object_set_type(res, INTEGER_TYPE);
        integer_set_ui(res->data, 1);
        return;
    }
}
void object__int(struct object_st *res, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = res->data;
    if (obj == NULL || obj->type == NULL) return;
    if (obj->type->convert != NULL && obj->type->convert->_int != NULL) {
        obj->type->convert->_int(res, obj->data);
    }
}
void object__float(struct object_st *res, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = res->data;
    if (obj == NULL || obj->type == NULL) return;
    if (obj->type->convert != NULL && obj->type->convert->_float != NULL) {
        obj->type->convert->_float(res, obj->data);
    }
}
void object__str(struct object_st *res, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = res->data;
    if (obj == NULL || obj->type == NULL) return;
    if (obj->type->convert != NULL && obj->type->convert->_str != NULL) {
        obj->type->convert->_str(res, obj->data);
    }
}