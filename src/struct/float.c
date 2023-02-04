#include "struct.h"
#include <sys/time.h>

struct object_math_op float_math = {NULL, NULL, METHOD_MATH &float__mul, METHOD_MATH &float__add, METHOD_MATH &float__sub, METHOD_MATH &float__div, NULL, NULL, METHOD_MATH &float__ls, METHOD_MATH &float__rs, METHOD_CONVERT &float__neg};
struct object_convert float_convert = {METHOD_CONVERT &float__bool, METHOD_CONVERT &float__int, METHOD_CONVERT &float__float, METHOD_CONVERT &float__str};
struct object_type float_type = {FLOAT_OP, NULL, &float_math, &float_convert};
// Standard operations
struct float_st *float_new() {
    struct float_st *res = skr_malloc(FLOAT_SIZE);
#ifdef USE_GMP
    mpf_init(res->mpz_int);
#else
    res->data = sub_float_new();
#endif
    return res;
}
void float_set(struct float_st *res, const struct float_st *a) {
    if (float_is_null(a)) return float_clear(res);
#ifdef USE_GMP
    mpf_set(res->mpz_int, a->mpz_int);
#else
    sub_float_set(res->data, a->data);
#endif
}
void float_clear(struct float_st *res) {
#ifdef USE_GMP
    mpf_set_ui(res->mpz_int, 0);
#else
    sub_float_clear(res->data);
#endif
}
void float_free(struct float_st *res) {
#ifdef USE_GMP
    mpf_clear(res->mpz_int);
#else
    sub_float_free(res->data);
#endif
    skr_free(res);
}
int float_cmp(const struct float_st *obj1, const struct float_st *obj2) {
#ifdef USE_GMP
    return mpf_cmp(obj1->mpz_int, obj2->mpz_int);
#else
    return sub_float_cmp(obj1->data, obj2->data);
#endif
}
int float_is_null(const struct float_st *res) {
    if (res == NULL) return 1;
#ifdef USE_GMP
    return (mpf_cmp_ui(res->mpz_int, 0) == 0);
#else
    return (res->data->size == 0);
#endif
}

// Class methods
void float_ls(struct float_st *res, const struct float_st *a, size_t num) {
#ifdef USE_GMP
    mpf_mul_2exp(res->mpz_int, a->mpz_int, num);
#else
    sub_float_ls(res->data, a->data, num);
#endif
}
void float_rs(struct float_st *res, const struct float_st *a, size_t num) {
#ifdef USE_GMP
    mpf_div_2exp(res->mpz_int, a->mpz_int, num);
#else
    sub_float_rs(res->data, a->data, num);
#endif
}

void float_add(struct float_st *res, const struct float_st *a, const struct float_st *b) {
#ifdef USE_GMP
    mpf_add(res->mpz_int, a->mpz_int, b->mpz_int);
#else
    sub_float_add(res->data, a->data, b->data);
#endif
}
void float_sub(struct float_st *res, const struct float_st *a, const struct float_st *b) {
#ifdef USE_GMP
    mpf_sub(res->mpz_int, a->mpz_int, b->mpz_int);
#else
    sub_float_sub(res->data, a->data, b->data);
#endif
}
void float_mul(struct float_st *res, const struct float_st *a, const struct float_st *b) {
#ifdef USE_GMP
    mpf_mul(res->mpz_int, a->mpz_int, b->mpz_int);
#else
    sub_float_mul(res->data, a->data, b->data);
#endif
}
void float_div(struct float_st *res, const struct float_st *a, const struct float_st *b) {
#ifdef USE_GMP
    mpf_div(res->mpz_int, a->mpz_int, b->mpz_int);
#else
    sub_float_div(res->data, a->data, b->data);
#endif
}
void float_neg(struct float_st *res, const struct float_st *a) {
#ifdef USE_GMP
    mpf_neg(res->mpz_int, a->mpz_int);
#else
    sub_integer_set(res->data, a->data);
    res->data->positive = 0;
#endif
}

// Math methods
void float__mul(struct object_st *res, const struct float_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != FLOAT_TYPE) return;
    object_set_type(res, FLOAT_TYPE);
    float_mul(res->data, obj1, obj2->data);
}
void float__add(struct object_st *res, const struct float_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != FLOAT_TYPE) return;
    object_set_type(res, FLOAT_TYPE);
    float_add(res->data, obj1, obj2->data);
}
void float__sub(struct object_st *res, const struct float_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != FLOAT_TYPE) return;
    object_set_type(res, FLOAT_TYPE);
    float_sub(res->data, obj1, obj2->data);
}
void float__div(struct object_st *res, const struct float_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != FLOAT_TYPE) return;
    object_set_type(res, FLOAT_TYPE);
    float_div(res->data, obj1, obj2->data);
}
void float__ls(struct object_st *res, const struct float_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, FLOAT_TYPE);
    int count = integer_get_si(obj2->data);
    if (count >= 0) float_ls(res->data, obj1, count);
    else float_rs(res->data, obj1, -count);
}
void float__rs(struct object_st *res, const struct float_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, FLOAT_TYPE);
    int count = integer_get_si(obj2->data);
    if (count >= 0) float_rs(res->data, obj1, count);
    else float_ls(res->data, obj1, -count);
}
void float__neg(struct object_st *res, const struct float_st *obj1){
    object_set_type(res, FLOAT_TYPE);
    float_neg(res->data, obj1);
}

// Convert methods
void float__bool(struct object_st *res, struct float_st *obj){
    object_set_type(res, INTEGER_TYPE);
    if(mpf_cmp_ui(obj->mpz_int, 0) == 0) integer_set_ui(res->data, 0);
    else integer_set_ui(res->data, 1);
}
void float__int(struct object_st *res, struct float_st *obj){
    // TODO
}
void float__float(struct object_st *res, struct float_st *obj){
    object_set_type(res, FLOAT_TYPE);
    float_set(res->data, obj);
}
void float__str(struct object_st *res, struct float_st *obj){
    // TODO
}