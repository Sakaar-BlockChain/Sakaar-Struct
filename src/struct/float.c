#include "struct.h"

struct object_math_op float_math = {NULL, NULL, METHOD_MATH &float__mul, METHOD_MATH &float__add, METHOD_MATH &float__sub, METHOD_MATH &float__div, NULL, NULL, METHOD_MATH &float__ls, METHOD_MATH &float__rs, METHOD_CONVERT &float__neg};
struct object_convert float_convert = {METHOD_CONVERT &float__bool, METHOD_CONVERT &float__int, METHOD_CONVERT &float__float, METHOD_CONVERT &float__str};
struct object_type float_type = {FLOAT_OP, NULL, NULL, &float_convert, &float_math};

// Convert Methods
void float__bool(struct object_st *res, struct object_st *err, struct float_st *obj){
    object_set_type(res, INTEGER_TYPE);
    if(float_is_null(obj)) integer_set_ui(res->data, 0);
    else integer_set_ui(res->data, 1);
}
void float__int(struct object_st *res, struct object_st *err, struct float_st *obj){
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Not Done", 8);
    // TODO
}
void float__float(struct object_st *res, struct object_st *err, struct float_st *obj){
    object_set_type(res, FLOAT_TYPE);
    float_set(res->data, obj);
}
void float__str(struct object_st *res, struct object_st *err, struct float_st *obj){
    object_set_type(res, STRING_TYPE);
    float_get_str(obj, res->data);
}

// Math Methods
void float__mul(struct object_st *res, struct object_st *err, const struct float_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 == NULL || obj2->type != FLOAT_TYPE) return;
    object_set_type(res, FLOAT_TYPE);
    float_mul(res->data, obj1, obj2->data);
}
void float__add(struct object_st *res, struct object_st *err, const struct float_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 == NULL || obj2->type != FLOAT_TYPE) return;
    object_set_type(res, FLOAT_TYPE);
    float_add(res->data, obj1, obj2->data);
}
void float__sub(struct object_st *res, struct object_st *err, const struct float_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 == NULL || obj2->type != FLOAT_TYPE) return;
    object_set_type(res, FLOAT_TYPE);
    float_sub(res->data, obj1, obj2->data);
}
void float__div(struct object_st *res, struct object_st *err, const struct float_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 == NULL || obj2->type != FLOAT_TYPE) return;
    object_set_type(res, FLOAT_TYPE);
    float_div(res->data, obj1, obj2->data);
}
void float__ls(struct object_st *res, struct object_st *err, const struct float_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, FLOAT_TYPE);
    int count = integer_get_si(obj2->data);
    if (count >= 0) float_ls(res->data, obj1, count);
    else float_rs(res->data, obj1, -count);
}
void float__rs(struct object_st *res, struct object_st *err, const struct float_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, FLOAT_TYPE);
    int count = integer_get_si(obj2->data);
    if (count >= 0) float_rs(res->data, obj1, count);
    else float_ls(res->data, obj1, -count);
}
void float__neg(struct object_st *res, struct object_st *err, const struct float_st *obj1) {
    object_set_type(res, FLOAT_TYPE);
    float_neg(res->data, obj1);
}