#include "struct.h"

struct object_tlv integer_tlv = {METHOD_GET_TLV &integer_get_tlv, METHOD_SET_TLV &integer_set_tlv};
struct object_math_op integer_math = {METHOD_MATH &integer__mod, METHOD_MATH &integer__and, METHOD_MATH &integer__mul, METHOD_MATH &integer__add, METHOD_MATH &integer__sub, METHOD_MATH &integer__div, METHOD_MATH &integer__xor, METHOD_MATH &integer__or, METHOD_MATH &integer__ls, METHOD_MATH &integer__rs, METHOD_CONVERT &integer__neg};
struct object_convert integer_convert = {METHOD_CONVERT &integer__bool, METHOD_CONVERT &integer__int, METHOD_CONVERT &integer__float, METHOD_CONVERT &integer__str};
struct object_type integer_type = {INTEGER_OP, &integer_tlv, &integer_math, &integer_convert};

// Math Methods
void integer__mod(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    integer_mod(res->data, obj1, obj2->data);
}
void integer__and(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    integer_and(res->data, obj1, obj2->data);
}
void integer__mul(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    integer_mul(res->data, obj1, obj2->data);
}
void integer__add(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    integer_add(res->data, obj1, obj2->data);
}
void integer__sub(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    integer_sub(res->data, obj1, obj2->data);
}
void integer__div(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    integer_div(res->data, obj1, obj2->data);
}
void integer__xor(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    integer_xor(res->data, obj1, obj2->data);
}
void integer__or(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    integer_or(res->data, obj1, obj2->data);
}
void integer__ls(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    int count = integer_get_si(obj2->data);
    if (count >= 0) integer_ls(res->data, obj1, count);
    else integer_rs(res->data, obj1, -count);
}
void integer__rs(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    int count = integer_get_si(obj2->data);
    if (count >= 0) integer_rs(res->data, obj1, count);
    else integer_ls(res->data, obj1, -count);
}
void integer__neg(struct object_st *res, const struct integer_st *obj1) {
    object_set_type(res, INTEGER_TYPE);
    integer_neg(res->data, obj1);
}

// Convert Methods
void integer__bool(struct object_st *res, struct integer_st *obj){
    object_set_type(res, INTEGER_TYPE);
    if(integer_is_null(obj)) integer_set_ui(res->data, 0);
    else integer_set_ui(res->data, 1);
}
void integer__int(struct object_st *res, struct integer_st *obj){
    object_set_type(res, INTEGER_TYPE);
    integer_set(res->data, obj);
}
void integer__float(struct object_st *res, struct integer_st *obj){
    // TODO
}
void integer__str(struct object_st *res, struct integer_st *obj){
    // TODO
}
