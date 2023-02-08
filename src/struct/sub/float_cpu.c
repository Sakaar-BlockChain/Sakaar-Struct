#include "struct.h"
#ifndef USE_GMP
// Standard operations
struct float_st *float_new() {
    struct float_st *res = skr_malloc(FLOAT_SIZE);
#ifdef USE_GMP
    mpf_init(res->mpz_int);
#else
    res->data = sub_float_new();i
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

// Class Methods
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
#endif