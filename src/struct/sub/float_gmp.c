#include "struct.h"
#ifdef USE_GMP
// Standard operations
struct float_st *float_new() {
    struct float_st *res = malloc(sizeof(struct float_st));
    mpf_init(res->mpz_int);
    return res;
}
void float_free(struct float_st *res) {
    if (res == NULL) return;
    mpf_clear(res->mpz_int);
    free(res);
}

void float_set(struct float_st *res, const struct float_st *a) {
    if (res == NULL) return;
    if (a == NULL) return float_clear(res);
    mpf_set(res->mpz_int, a->mpz_int);
}
void float_copy(struct float_st *res, const struct float_st *a) {
    if (res == NULL) return;
    if (a == NULL) return float_clear(res);
    mpf_set(res->mpz_int, a->mpz_int);
}

void float_clear(struct float_st *res) {
    if (res == NULL) return;
    mpf_set_ui(res->mpz_int, 0);
}
int8_t float_cmp(const struct float_st *obj1, const struct float_st *obj2) {
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    return mpf_cmp(obj1->mpz_int, obj2->mpz_int);
}

// Cmp Methods
int8_t float_is_null(const struct float_st *res) {
    if (res == NULL) return 1;
    return (int8_t) (mpf_cmp_ui(res->mpz_int, 0) == 0);
}

// Class Methods
void float_ls(struct float_st *res, const struct float_st *a, size_t num) {
    mpf_mul_2exp(res->mpz_int, a->mpz_int, num);
}
void float_rs(struct float_st *res, const struct float_st *a, size_t num) {
    mpf_div_2exp(res->mpz_int, a->mpz_int, num);
}

void float_add(struct float_st *res, const struct float_st *a, const struct float_st *b) {
    mpf_add(res->mpz_int, a->mpz_int, b->mpz_int);
}
void float_sub(struct float_st *res, const struct float_st *a, const struct float_st *b) {
    mpf_sub(res->mpz_int, a->mpz_int, b->mpz_int);
}
void float_mul(struct float_st *res, const struct float_st *a, const struct float_st *b) {
    mpf_mul(res->mpz_int, a->mpz_int, b->mpz_int);
}
void float_div(struct float_st *res, const struct float_st *a, const struct float_st *b) {
    mpf_div(res->mpz_int, a->mpz_int, b->mpz_int);
}
void float_neg(struct float_st *res, const struct float_st *a) {
    mpf_neg(res->mpz_int, a->mpz_int);
}

// String Methods
void float_set_str(struct float_st *res, const struct string_st *str) {
    //TODO
}
void float_get_str(const struct float_st *res, struct string_st *str) {
    //TODO
}
#endif