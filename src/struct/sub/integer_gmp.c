#include "struct.h"
#include <sys/time.h>
#ifdef USE_GMP
// Standard operations
struct integer_st *integer_new() {
    struct integer_st *res = skr_malloc(INTEGER_SIZE);
    mpz_init(res->mpz_int);

    return res;
}
void integer_set(struct integer_st *res, const struct integer_st *a) {
    if (integer_is_null(a)) return integer_clear(res);
    mpz_set(res->mpz_int, a->mpz_int);

}
void integer_clear(struct integer_st *res) {
    mpz_set_ui(res->mpz_int, 0);

}
void integer_free(struct integer_st *res) {
    mpz_clear(res->mpz_int);

    skr_free(res);
}
int integer_cmp(const struct integer_st *obj1, const struct integer_st *obj2) {
    return mpz_cmp(obj1->mpz_int, obj2->mpz_int);

}

// Class Methods
void integer_ls(struct integer_st *res, const struct integer_st *a, size_t num) {
    mpz_mul_2exp(res->mpz_int, a->mpz_int, num);

}
void integer_rs(struct integer_st *res, const struct integer_st *a, size_t num) {
    mpz_div_2exp(res->mpz_int, a->mpz_int, num);

}
void integer_and(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
    mpz_and(res->mpz_int, a->mpz_int, b->mpz_int);

}
void integer_xor(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
    mpz_xor(res->mpz_int, a->mpz_int, b->mpz_int);

}
void integer_or(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
    mpz_ior(res->mpz_int, a->mpz_int, b->mpz_int);

}

void integer_add(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
    mpz_add(res->mpz_int, a->mpz_int, b->mpz_int);

}
void integer_sub(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
    mpz_sub(res->mpz_int, a->mpz_int, b->mpz_int);

}
void integer_mul(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
    mpz_mul(res->mpz_int, a->mpz_int, b->mpz_int);

}
void integer_div(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
    mpz_div(res->mpz_int, a->mpz_int, b->mpz_int);

}
void integer_mod(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
    mpz_mod(res->mpz_int, a->mpz_int, b->mpz_int);

}
void integer_inv(struct integer_st *res, const struct integer_st *a, const struct integer_st *mod) {
    mpz_invert(res->mpz_int, a->mpz_int, mod->mpz_int);

}
void integer_powm(struct integer_st *res, const struct integer_st *a, const struct integer_st *b, const struct integer_st *mod) {
    mpz_powm(res->mpz_int, a->mpz_int, b->mpz_int, mod->mpz_int);

}
void integer_neg(struct integer_st *res, const struct integer_st *a) {
    mpz_neg(res->mpz_int, a->mpz_int);
}

// Data init Methods
void integer_set_ui(struct integer_st *res, unsigned number) {
    mpz_set_ui(res->mpz_int, number);

}
unsigned integer_get_ui(const struct integer_st *res) {
    return mpz_get_ui(res->mpz_int);

}
signed integer_get_si(const struct integer_st *res) {
    return mpz_get_si(res->mpz_int);

}

// Hex Methods
void _integer_set_str(struct integer_st *res, const char *str, size_t size) {
    if (str == NULL) integer_clear(res);
    if(str == NULL) return mpz_set_ui(res->mpz_int, 0);
    struct string_st *temp = string_new();
    string_set_str(temp, str, size);
    if (string_is_null(temp)) integer_clear(res);
    if(temp->data != NULL) mpz_set_str(res->mpz_int, temp->data, 16);
    else mpz_set_ui(res->mpz_int, 0);
    string_free(temp);
}
void integer_set_str(struct integer_st *res, const struct string_st *str) {
    if (string_is_null(str)) integer_clear(res);
    if(str->data != NULL) mpz_set_str(res->mpz_int, str->data, 16);
    else mpz_set_ui(res->mpz_int, 0);
}
void integer_get_str(const struct integer_st *res, struct string_st *str) {
    if(str == NULL || res == NULL) return;
    size_t str_len = mpz_sizeinbase(res->mpz_int, 16);
    size_t is_neg = (mpz_sgn(res->mpz_int) < 0);

    string_resize(str, str_len + is_neg);
    mpz_get_str(str->data, 16, res->mpz_int);
}
void integer_set_time(struct integer_st *res) {
    if (res == NULL) return;
    struct timeval te;
    gettimeofday(&te, NULL);
    long long seconds = te.tv_sec*1000LL + te.tv_usec/1000;

    mpz_set_si(res->mpz_int, (int)(seconds >> 32));     /* n = (int)sll >> 32 */
    mpz_mul_2exp(res->mpz_int, res->mpz_int, 32);             /* n <<= 32 */
    mpz_add_ui(res->mpz_int, res->mpz_int, (unsigned int)seconds);
}


void _integer_set_str_dec(struct integer_st *res, const char *str, size_t size) {
    if (str == NULL) integer_clear(res);
    if(str == NULL) return mpz_set_ui(res->mpz_int, 0);
    struct string_st *temp = string_new();
    string_set_str(temp, str, size);
    if (string_is_null(temp)) integer_clear(res);
    if(temp->data != NULL) mpz_set_str(res->mpz_int, temp->data, 10);
    else mpz_set_ui(res->mpz_int, 0);
    string_free(temp);

}
void _integer_set_str_oct(struct integer_st *res, const char *str, size_t size) {
    if (str == NULL) integer_clear(res);
    if(str == NULL) return mpz_set_ui(res->mpz_int, 0);
    struct string_st *temp = string_new();
    string_set_str(temp, str, size);
    if (string_is_null(temp)) integer_clear(res);
    if(temp->data != NULL) mpz_set_str(res->mpz_int, temp->data, 8);
    else mpz_set_ui(res->mpz_int, 0);
    string_free(temp);

}
void _integer_set_str_bin(struct integer_st *res, const char *str, size_t size) {
    if (str == NULL) integer_clear(res);
    if(str == NULL) return mpz_set_ui(res->mpz_int, 0);
    struct string_st *temp = string_new();
    string_set_str(temp, str, size);
    if (string_is_null(temp)) integer_clear(res);
    if(temp->data != NULL) mpz_set_str(res->mpz_int, temp->data, 2);
    else mpz_set_ui(res->mpz_int, 0);
    string_free(temp);
}
// Cmp Methods
int integer_is_null(const struct integer_st *res) {
    if (res == NULL) return 1;
    return (mpz_cmp_ui(res->mpz_int, 0) == 0);

}
int integer_is_neg(const struct integer_st *res) {
    if (res == NULL) return 1;
    return (mpz_cmp_ui(res->mpz_int, 0) < 0);

}
void integer_random(struct integer_st *res, const struct integer_st *a) {
    if (res == NULL) return;
    if (integer_is_null(a)) return integer_clear(res);
    struct timeval te;
    gettimeofday(&te, NULL);
    long long seed = te.tv_sec * 1000LL + te.tv_usec / 1000;
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
    mpz_urandomm(res->mpz_int, state, a->mpz_int);

}

// TLV Methods
void _integer_set_tlv(struct integer_st *res, const struct string_st *tlv) {
    if (res == NULL) return;
    integer_clear(res);
    if (string_is_null(tlv)) return;
    char *data = tlv_get_value(tlv->data);
    size_t size = tlv_get_size(tlv->data);
    size_t is_neg = (data[0] == 0x00);
    size_t num_len = (size - is_neg) * 2;

    char *temp = skr_malloc(num_len + is_neg + 1);
    for(size_t i = 0; i < num_len + is_neg + 1; i++) temp[i] = 0;
    if (is_neg) temp[0] = '-';

    for (size_t i = 0; i < size; i++) {
        temp[i*2 + is_neg] = get_char_16(((unsigned)data[i] >> 4) & 0xF);
        temp[i*2+1 + is_neg] = get_char_16((unsigned char)data[i] & 0x0F);
    }

    mpz_set_str(res->mpz_int, temp, 16);
    skr_free(temp);
}
void _integer_get_tlv(const struct integer_st *res, struct string_st *tlv, unsigned tag) {
    if (tlv == NULL) return;
    string_clear(tlv);
    if (res == NULL) return;
    size_t num_len = mpz_sizeinbase(res->mpz_int, 16);
    size_t is_neg = (mpz_sgn(res->mpz_int) == -1);
    size_t tlv_len = (num_len + 1) / 2;

    char *temp = skr_malloc(num_len + is_neg + 1);
    mpz_get_str(temp, 16, res->mpz_int);

    string_resize(tlv, tlv_len + is_neg);

    for (size_t i = 0; i < num_len; i++){
        size_t id = tlv_len - (num_len - i - 1)/ 2 - 1 + is_neg;
        tlv->data[id] = (char)((tlv->data[id] << 4) | set_char_16(temp[is_neg + i]));
    }

    skr_free(temp);
    tlv_set_string(tlv, tag, tlv);
}
void integer_set_tlv(struct integer_st *res, const struct string_st *tlv) {
    if (res == NULL) return;
    integer_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != INTEGER_TLV) return;
    char *data = tlv_get_value(tlv->data);
    size_t size = tlv_get_size(tlv->data);
    size_t is_neg = (data[0] == 0x00);
    size_t num_len = (size - is_neg) * 2;

    char *temp = skr_malloc(num_len + is_neg + 1);
    for(size_t i = 0; i < num_len + is_neg + 1; i++) temp[i] = 0;
    if (is_neg) temp[0] = '-';

    for (size_t i = 0; i < size; i++) {
        temp[i*2 + is_neg] = get_char_16(((unsigned)data[i] >> 4) & 0xF);
        temp[i*2+1 + is_neg] = get_char_16((unsigned char)data[i] & 0x0F);
    }

    mpz_set_str(res->mpz_int, temp, 16);
    skr_free(temp);
}
void integer_get_tlv(const struct integer_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    string_clear(tlv);
    if (res == NULL) return;
    size_t num_len = mpz_sizeinbase(res->mpz_int, 16);
    size_t is_neg = (mpz_sgn(res->mpz_int) == -1);
    size_t tlv_len = (num_len + 1) / 2;

    char *temp = skr_malloc(num_len + is_neg + 1);
    mpz_get_str(temp, 16, res->mpz_int);

    string_resize(tlv, tlv_len + is_neg);

    for (size_t i = 0; i < num_len; i++){
        size_t id = tlv_len - (num_len - i - 1)/ 2 - 1 + is_neg;
        tlv->data[id] = (char)((tlv->data[id] << 4) | set_char_16(temp[is_neg + i]));
    }

    skr_free(temp);
    tlv_set_string(tlv, INTEGER_TLV, tlv);
}
#endif