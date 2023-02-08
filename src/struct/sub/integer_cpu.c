#include "struct.h"
#include <sys/time.h>
#ifndef USE_GMP
// Standard operations
struct integer_st *integer_new() {
    struct integer_st *res = skr_malloc(INTEGER_SIZE);
#ifdef USE_GMP
    mpz_init(res->mpz_int);
#else
    res->data = sub_integer_new();
#endif
    return res;
}
void integer_set(struct integer_st *res, const struct integer_st *a) {
    if (integer_is_null(a)) return integer_clear(res);
#ifdef USE_GMP
    mpz_set(res->mpz_int, a->mpz_int);
#else
    sub_integer_set(res->data, a->data);
#endif
}
void integer_clear(struct integer_st *res) {
#ifdef USE_GMP
    mpz_set_ui(res->mpz_int, 0);
#else
    sub_integer_clear(res->data);
#endif
}
void integer_free(struct integer_st *res) {
#ifdef USE_GMP
    mpz_clear(res->mpz_int);
#else
    sub_integer_free(res->data);
#endif
    skr_free(res);
}
int integer_cmp(const struct integer_st *obj1, const struct integer_st *obj2) {
#ifdef USE_GMP
    return mpz_cmp(obj1->mpz_int, obj2->mpz_int);
#else
    return sub_integer_cmp(obj1->data, obj2->data);
#endif
}

// Class Methods
void integer_ls(struct integer_st *res, const struct integer_st *a, size_t num) {
#ifdef USE_GMP
    mpz_mul_2exp(res->mpz_int, a->mpz_int, num);
#else
    sub_integer_ls(res->data, a->data, num);
#endif
}
void integer_rs(struct integer_st *res, const struct integer_st *a, size_t num) {
#ifdef USE_GMP
    mpz_div_2exp(res->mpz_int, a->mpz_int, num);
#else
    sub_integer_rs(res->data, a->data, num);
#endif
}
void integer_and(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
#ifdef USE_GMP
    mpz_and(res->mpz_int, a->mpz_int, b->mpz_int);
#else
    sub_integer_and(res->data, a->data, b->data);
#endif
}
void integer_xor(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
#ifdef USE_GMP
    mpz_xor(res->mpz_int, a->mpz_int, b->mpz_int);
#else
    sub_integer_xor(res->data, a->data, b->data);
#endif
}
void integer_or(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
#ifdef USE_GMP
    mpz_ior(res->mpz_int, a->mpz_int, b->mpz_int);
#else
    sub_integer_or(res->data, a->data, b->data);
#endif
}

void integer_add(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
#ifdef USE_GMP
    mpz_add(res->mpz_int, a->mpz_int, b->mpz_int);
#else
    sub_integer_add(res->data, a->data, b->data);
#endif
}
void integer_sub(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
#ifdef USE_GMP
    mpz_sub(res->mpz_int, a->mpz_int, b->mpz_int);
#else
    sub_integer_sub(res->data, a->data, b->data);
#endif
}
void integer_mul(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
#ifdef USE_GMP
    mpz_mul(res->mpz_int, a->mpz_int, b->mpz_int);
#else
    sub_integer_mul(res->data, a->data, b->data);
#endif
}
void integer_div(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
#ifdef USE_GMP
    mpz_div(res->mpz_int, a->mpz_int, b->mpz_int);
#else
    sub_integer_div(res->data, a->data, b->data);
#endif
}
void integer_mod(struct integer_st *res, const struct integer_st *a, const struct integer_st *b) {
#ifdef USE_GMP
    mpz_mod(res->mpz_int, a->mpz_int, b->mpz_int);
#else
    sub_integer_mod(res->data, a->data, b->data);
#endif
}
void integer_inv(struct integer_st *res, const struct integer_st *a, const struct integer_st *mod) {
#ifdef USE_GMP
    mpz_invert(res->mpz_int, a->mpz_int, mod->mpz_int);
#else
    sub_integer_inv(res->data, a->data, mod->data);
#endif
}
void integer_powm(struct integer_st *res, const struct integer_st *a, const struct integer_st *b, const struct integer_st *mod) {
#ifdef USE_GMP
    mpz_powm(res->mpz_int, a->mpz_int, b->mpz_int, mod->mpz_int);
#else
    sub_integer_powm(res->data, a->data, b->data, mod->data);
#endif
}
void integer_neg(struct integer_st *res, const struct integer_st *a) {
#ifdef USE_GMP
    mpz_neg(res->mpz_int, a->mpz_int);
#else
    sub_integer_set(res->data, a->data);
    res->data->positive = 0;
#endif
}

// Data init Methods
void integer_set_ui(struct integer_st *res, unsigned number) {
#ifdef USE_GMP
    mpz_set_ui(res->mpz_int, number);
#else
    sub_integer_set_ui(res->data, number);
#endif
}
unsigned integer_get_ui(const struct integer_st *res) {
#ifdef USE_GMP
    return mpz_get_ui(res->mpz_int);
#else
    return sub_integer_get_ui(res->data);
#endif
}
signed integer_get_si(const struct integer_st *res) {
#ifdef USE_GMP
    return mpz_get_si(res->mpz_int);
#else
    return sub_integer_get_si(res->data);
#endif
}

// Hex Methods
void _integer_set_str(struct integer_st *res, const char *str, size_t size) {
    if (str == NULL) integer_clear(res);
#ifdef USE_GMP
    if(str == NULL) return mpz_set_ui(res->mpz_int, 0);
    struct string_st *temp = string_new();
    string_set_str(temp, str, size);
    if (string_is_null(temp)) integer_clear(res);
    if(temp->data != NULL) mpz_set_str(res->mpz_int, temp->data, 16);
    else mpz_set_ui(res->mpz_int, 0);
    string_free(temp);
#else
    size_t type_size = 4;
    size_t dif = (16 / type_size);
    size_t j = 0;
    if(size > 0 && str[0] == '-'){
        size--;
        j++;
        res->data->positive = 0;
    }
    size_t _size = size / dif + (size_t) (size % dif != 0);
    sub_integer_resize(res->data, _size);
    for (size_t i = 0; i < _size; i++) res->data->data[i] = 0;
    for (size_t i = size; i > 0; j++) {
        i--;
        res->data->data[i / dif] <<= type_size;
        res->data->data[i / dif] |= set_char_16(str[j]);
    }
    sub_integer_fit(res->data);
#endif
}
void integer_set_str(struct integer_st *res, const struct string_st *str) {
    if (string_is_null(str)) integer_clear(res);
#ifdef USE_GMP
    if(str->data != NULL) mpz_set_str(res->mpz_int, str->data, 16);
    else mpz_set_ui(res->mpz_int, 0);
#else
    char *data = str->data;
    size_t size = str->size;

    size_t type_size = 4;
    size_t dif = (16 / type_size);
    size_t j = 0;
    if(size > 0 && data[0] == '-'){
        size--;
        j++;
        res->data->positive = 0;
    }
    size_t _size = size / dif + (size_t) (size % dif != 0);
    sub_integer_resize(res->data, _size);
    for (size_t i = 0; i < _size; i++) res->data->data[i] = 0;
    for (size_t i = size; i > 0; j++) {
        i--;
        res->data->data[i / dif] <<= type_size;
        res->data->data[i / dif] |= set_char_16(data[j]);
    }
    sub_integer_fit(res->data);
#endif
}
void integer_get_str(const struct integer_st *res, struct string_st *str) {
    if(str == NULL || res == NULL) return;
#ifdef USE_GMP
    size_t str_len = mpz_sizeinbase(res->mpz_int, 16);
    size_t is_neg = (mpz_sgn(res->mpz_int) < 0);

    string_resize(str, str_len + is_neg);
    mpz_get_str(str->data, 16, res->mpz_int);
#else
    size_t type_size = 4;
    size_t dif = (16 / type_size);
    size_t size = (res->data->size * dif);

    char *data = skr_malloc(size + 2);

    size_t j = 0;
    if(!res->data->positive) data[j++] = '-';
    unsigned short temp, used = 0;
    for (size_t i = 0; i < res->data->size; i++) {
        for(size_t k=dif; k>0;k--){
            temp = res->data->data[res->data->size - i - 1];
            for(size_t _k = 1; _k < k;_k++) temp >>= type_size;
            temp %= (1<<type_size);
            if(temp != 0 || used != 0){
                data[j++] = get_char_16(temp);
                used = 1;
            }
        }
    }
    string_set_str(str, data, j);
    skr_free(data);
#endif
}
void integer_set_time(struct integer_st *res) {
    if (res == NULL) return;
    struct timeval te;
    gettimeofday(&te, NULL);
    long long seconds = te.tv_sec*1000LL + te.tv_usec/1000;

#ifdef USE_GMP
    mpz_set_si(res->mpz_int, (int)(seconds >> 32));     /* n = (int)sll >> 32 */
    mpz_mul_2exp(res->mpz_int, res->mpz_int, 32);             /* n <<= 32 */
    mpz_add_ui(res->mpz_int, res->mpz_int, (unsigned int)seconds);
#else
    sub_integer_resize(res, 4);
    res->data->positive = 1;
    res->data->data[0] = (unsigned short) (seconds % 65536); seconds >>= 16;
    res->data->data[1] = (unsigned short) (seconds % 65536); seconds >>= 16;
    res->data->data[2] = (unsigned short) (seconds % 65536); seconds >>= 16;
    res->data->data[3] = (unsigned short) (seconds % 65536); seconds >>= 16;
    sub_integer_fit(res);
#endif
}

void _integer_set_str_dec(struct integer_st *res, const char *str, size_t size) {
    if (str == NULL) integer_clear(res);
#ifdef USE_GMP
    if(str == NULL) return mpz_set_ui(res->mpz_int, 0);
    struct string_st *temp = string_new();
    string_set_str(temp, str, size);
    if (string_is_null(temp)) integer_clear(res);
    if(temp->data != NULL) mpz_set_str(res->mpz_int, temp->data, 10);
    else mpz_set_ui(res->mpz_int, 0);
    string_free(temp);
#else
    // TODO set_str_dec
#endif
}
void _integer_set_str_oct(struct integer_st *res, const char *str, size_t size) {
    if (str == NULL) integer_clear(res);
#ifdef USE_GMP
    if(str == NULL) return mpz_set_ui(res->mpz_int, 0);
    struct string_st *temp = string_new();
    string_set_str(temp, str, size);
    if (string_is_null(temp)) integer_clear(res);
    if(temp->data != NULL) mpz_set_str(res->mpz_int, temp->data, 8);
    else mpz_set_ui(res->mpz_int, 0);
    string_free(temp);
#else
    // TODO set_str_oct
#endif
}
void _integer_set_str_bin(struct integer_st *res, const char *str, size_t size) {
    if (str == NULL) integer_clear(res);
#ifdef USE_GMP
    if(str == NULL) return mpz_set_ui(res->mpz_int, 0);
    struct string_st *temp = string_new();
    string_set_str(temp, str, size);
    if (string_is_null(temp)) integer_clear(res);
    if(temp->data != NULL) mpz_set_str(res->mpz_int, temp->data, 2);
    else mpz_set_ui(res->mpz_int, 0);
    string_free(temp);
#else
    size_t type_size = 1;
    size_t dif = (16 / type_size);
    size_t j = 0;
    if(size > 0 && str[0] == '-'){
        size--;
        j++;
        res->data->positive = 0;
    }
    size_t _size = size / dif + (size_t) (size % dif != 0);
    sub_integer_resize(res->data, _size);
    for (size_t i = 0; i < _size; i++) res->data->data[i] = 0;
    for (size_t i = size; i > 0; j++) {
        i--;
        res->data->data[i / dif] <<= type_size;
        res->data->data[i / dif] |= set_char_16(str[j]);
    }
    sub_integer_fit(res->data);
#endif
}
// Cmp Methods
int integer_is_null(const struct integer_st *res) {
    if (res == NULL) return 1;
#ifdef USE_GMP
    return (mpz_cmp_ui(res->mpz_int, 0) == 0);
#else
    return (res->data->size == 0);
#endif
}
int integer_is_neg(const struct integer_st *res) {
    if (res == NULL) return 1;
#ifdef USE_GMP
    return (mpz_cmp_ui(res->mpz_int, 0) < 0);
#else
    return (res->data->positive == 0);
#endif
}
void integer_random(struct integer_st *res, const struct integer_st *a) {
    if (res == NULL) return;
    if (integer_is_null(a)) return integer_clear(res);
#ifdef USE_GMP
    struct timeval te;
    gettimeofday(&te, NULL);
    long long seed = te.tv_sec * 1000LL + te.tv_usec / 1000;
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
    mpz_urandomm(res->mpz_int, state, a->mpz_int);
#else
    return (res->data->size == 0);
#endif
}

// TLV Methods
void _integer_set_tlv(struct integer_st *res, const struct string_st *tlv) {
    if (res == NULL) return;
    integer_clear(res);
    if (string_is_null(tlv)) return;
#ifdef USE_GMP
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
#else
    char *data = tlv_get_value(tlv->data);
    size_t size = tlv_get_size(tlv->data);

    size_t type_size = 8;
    size_t dif = (16 / type_size);
    size_t j = 0;
    if(size > 0 && data[0] == 0x00){
        size--;
        j++;
        res->data->positive = 0;
    }
    size_t _size = size / dif + (size_t) (size % dif != 0);
    sub_integer_resize(res->data, _size);
    for (size_t i = 0; i < _size; i++) res->data->data[i] = 0;
    for (size_t i = size; i > 0; j++) {
        i--;
        res->data->data[i / dif] <<= type_size;
        res->data->data[i / dif] |= (unsigned char)data[j];
    }
    sub_integer_fit(res->data);
#endif
}
void _integer_get_tlv(const struct integer_st *res, struct string_st *tlv, unsigned tag) {
    if (tlv == NULL) return;
    string_clear(tlv);
    if (res == NULL) return;
#ifdef USE_GMP
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
#else
    size_t type_size = 8;
    size_t dif = (16 / type_size);
    size_t size = (res->data->size * dif);

    char *data = skr_malloc(size + 2);

    size_t j = 0;
    if(!res->data->positive) data[j++] = 0x00;
    unsigned short temp, used = 0;
    for (size_t i = 0; i < res->data->size; i++) {
        for(size_t k=dif; k>0;k--){
            temp = res->data->data[res->data->size - i - 1];
            for(size_t _k = 1; _k < k;_k++) temp >>= type_size;
            temp %= (1<<type_size);
            if(temp != 0 || used != 0){
                data[j++] = (char)(unsigned char)temp;
                used = 1;
            }
        }
    }
    tlv_set_str(tlv, tag, data, j);
    skr_free(data);
#endif
}
void integer_set_tlv(struct integer_st *res, const struct string_st *tlv) {
    if (res == NULL) return;
    integer_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != INTEGER_TLV) return;
#ifdef USE_GMP
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
#else
    char *data = tlv_get_value(tlv->data);
    size_t size = tlv_get_size(tlv->data);

    size_t type_size = 8;
    size_t dif = (16 / type_size);
    size_t j = 0;
    if(size > 0 && data[0] == 0x00){
        size--;
        j++;
        res->data->positive = 0;
    }
    size_t _size = size / dif + (size_t) (size % dif != 0);
    sub_integer_resize(res->data, _size);
    for (size_t i = 0; i < _size; i++) res->data->data[i] = 0;
    for (size_t i = size; i > 0; j++) {
        i--;
        res->data->data[i / dif] <<= type_size;
        res->data->data[i / dif] |= (unsigned char)data[j];
    }
    sub_integer_fit(res->data);
#endif
}
void integer_get_tlv(const struct integer_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    string_clear(tlv);
    if (res == NULL) return;
#ifdef USE_GMP
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
#else
    size_t type_size = 8;
    size_t dif = (16 / type_size);
    size_t size = (res->data->size * dif);

    char *data = skr_malloc(size + 2);

    size_t j = 0;
    if(!res->data->positive) data[j++] = 0x00;
    unsigned short temp, used = 0;
    for (size_t i = 0; i < res->data->size; i++) {
        for(size_t k=dif; k>0;k--){
            temp = res->data->data[res->data->size - i - 1];
            for(size_t _k = 1; _k < k;_k++) temp >>= type_size;
            temp %= (1<<type_size);
            if(temp != 0 || used != 0){
                data[j++] = (char)(unsigned char)temp;
                used = 1;
            }
        }
    }
    tlv_set_str(tlv, INTEGER_TLV, data, j);
    skr_free(data);
#endif
}
#endif