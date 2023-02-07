#include "struct.h"
#ifdef USE_GMP
// Standard operations
struct float_st *float_new() {
    struct float_st *res = skr_malloc(FLOAT_SIZE);
    mpf_init(res->mpz_int);
    return res;
}
void float_set(struct float_st *res, const struct float_st *a) {
    if (float_is_null(a)) return float_clear(res);
    mpf_set(res->mpz_int, a->mpz_int);
}
void float_clear(struct float_st *res) {
    mpf_set_ui(res->mpz_int, 0);

}
void float_free(struct float_st *res) {
    mpf_clear(res->mpz_int);
    skr_free(res);
}
int float_cmp(const struct float_st *obj1, const struct float_st *obj2) {
    return mpf_cmp(obj1->mpz_int, obj2->mpz_int);

}
int float_is_null(const struct float_st *res) {
    if (res == NULL) return 1;
    return (mpf_cmp_ui(res->mpz_int, 0) == 0);
}

// Class methods
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
#endif