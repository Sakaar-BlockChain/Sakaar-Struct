#include "struct.h"

struct object_type sub_integer_type = {SUB_INTEGER_OP, METHOD_GET_TLV &sub_integer_get_tlv, METHOD_SET_TLV &sub_integer_set_tlv};

void sub_integer_resize(struct sub_integer *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->max_size = size;
        res->data = skr_malloc(size * sizeof(unsigned short) + 1);
        for (size_t i = 0; i < size; i++) res->data[i] = 0;
    } else if (res->max_size < size) {
        res->data = skr_realloc( res->data, sizeof(unsigned short) * size + 1);
        for (size_t i = res->max_size; i < size; i++) res->data[i] = 0;
        res->max_size = size;
    }
    if (res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            res->data[i] = 0;
        }
    }
    res->size = size;
}
void sub_integer_fit(struct sub_integer *res) {
    while (res->size != 0 && res->data[res->size - 1] == 0) {
        res->size--;
    }
    if (res->size == 0) (res)->positive = 1;
}

#define sub_integer_add_pos(res, a, b, al, bl) { \
    unsigned short temp = 0, temp2; \
    sub_integer_resize(res, (al) + 1); \
    for (size_t i = 0; i < (al); i++) { \
        temp2 = (a)[i] + temp; \
        if (i < (bl)) temp2 += (b)[i]; \
        temp = (temp2 < (a)[i] || (temp2 == (a)[i] && (temp != 0 || ((bl) > i && (b)[i] != 0)))); \
        (res)->data[i] = temp2; \
    } \
    (res)->data[al] = temp; \
    sub_integer_fit(res); \
}

#define sub_integer_sub_pos(res, a, b, al, bl) { \
    unsigned short temp = 0, temp2; \
    sub_integer_resize(res, (al)); \
    for (size_t i = 0; i < (al); i++) { \
        temp2 = (a)[i] - temp; \
        if (i < (bl)) temp2 -= (b)[i]; \
        temp = (temp2 > (a)[i] || (temp2 == (a)[i] && (temp != 0 || ((bl) > i && (b)[i] != 0)))); \
        (res)->data[i] = temp2; \
    } \
    sub_integer_fit(res); \
}

int sub_integer_cmp_pos(const unsigned short *a, const unsigned short *b, size_t al, size_t bl) {
    if (al != bl) {
        if (al > bl) return 1;
        return -1;
    }
    for (size_t i = al; i > 0;) {
        i--;
        if (a[i] != b[i]) {
            if (a[i] > b[i]) return 1;
            return -1;
        }
    }
    return 0;
}

void sub_integer_mul_pos(struct sub_integer *res, const unsigned short *a, const unsigned short *b, const unsigned al, const unsigned bl) {
    size_t n = al;
    if (bl > n) n = bl;
    if (al == 0 || bl == 0 || n == 0) {
        sub_integer_resize(res, 0);
        return;
    }
    if (n == 1) {
        unsigned res1 = ((unsigned) a[0] * (unsigned) b[0]);
        sub_integer_resize(res, 2);
        res->data[0] = (unsigned short) (res1 % (1 << 16));
        res->data[1] = (unsigned short) (res1 / (1 << 16));
        sub_integer_fit(res);
        return;
    }
    size_t mid = n / 2;
    if (al < mid) {
        struct sub_integer *R = sub_integer_new();
        struct sub_integer *L = sub_integer_new();
        sub_integer_mul_pos(R, a, b, al, mid);
        sub_integer_mul_pos(L, a, b + mid, al, bl - mid);
        sub_integer_ls(L, L, mid * 16);
        sub_integer_add_pos(res, L->data, R->data, L->size, R->size)
        sub_integer_free(R);
        sub_integer_free(L);
        return;
    } else if (bl < mid) {
        struct sub_integer *R = sub_integer_new();
        struct sub_integer *L = sub_integer_new();
        sub_integer_mul_pos(R, a, b, mid, bl);
        sub_integer_mul_pos(L, a + mid, b, al - mid, bl);
        sub_integer_ls(L, L, mid * 16);
        sub_integer_add_pos(res, L->data, R->data, L->size, R->size)
        sub_integer_free(R);
        sub_integer_free(L);
        return;
    }
    struct sub_integer *S = sub_integer_new();
    struct sub_integer *A = sub_integer_new();
    struct sub_integer *B = sub_integer_new();
    {
        if (al - mid >= mid) sub_integer_add_pos(A, a + mid, a, al - mid, mid)
        else sub_integer_add_pos(A, a, a + mid, mid, al - mid)
        if (bl - mid >= mid) sub_integer_add_pos(B, b + mid, b, bl - mid, mid)
        else sub_integer_add_pos(B, b, b + mid, mid, bl - mid)
        sub_integer_mul_pos(S, A->data, B->data, A->size, B->size);
//        sub_integer_clear(A);
//        sub_integer_clear(B);
    }
    sub_integer_mul_pos(A, a, b, mid, mid);
    sub_integer_mul_pos(B, a + mid, b + mid, al - mid, bl - mid);
    sub_integer_sub_pos(S, S->data, A->data, S->size, A->size)
    sub_integer_sub_pos(S, S->data, B->data, S->size, B->size)
    sub_integer_ls(B, B, mid * 16 * 2);
    sub_integer_ls(S, S, mid * 16);
    sub_integer_add(res, B, S);
    sub_integer_add(res, res, A);
    sub_integer_free(A);
    sub_integer_free(B);
    sub_integer_free(S);
}

void sub_integer_ls(struct sub_integer *res, const struct sub_integer *a, size_t num) {
    size_t shift_m = num / 16 + (size_t) (num % 16 != 0);
    size_t nl = shift_m + a->size;
    unsigned short *array = skr_malloc(sizeof(unsigned short) * (nl + 1));
    for (size_t i = 0; i < shift_m; i++) {
        array[i] = 0;
    }
    for (size_t i = 0; i < a->size; i++) {
        array[i + shift_m] = a->data[i];
    }
    if (num % 16 != 0) {
        unsigned short mask_m = (1 << (16 - num % 16));
        unsigned short mask_l = mask_m - 1;
        for (size_t i = 0; i < nl; i++) {
            array[i] >>= (16 - num % 16);
            if (i + 1 < nl) {
                array[i] |= ((array[i + 1] & mask_l) << num % 16);
            }
        }
    }
    if (res->data == NULL) {
        res->data = array;
        res->max_size = res->size = nl;
    } else {
        skr_free(res->data);
        res->data = array;
        res->max_size = res->size = nl;
    }
    res->positive = a->positive;
    sub_integer_fit(res);
}
void sub_integer_rs(struct sub_integer *res, const struct sub_integer *a, size_t num) {
    size_t shift_m = num / 16;
    size_t nl = a->size - shift_m;
    if (num / 16 > a->size) nl = 0;
    unsigned short *array = skr_malloc(sizeof(unsigned short) * (nl + 1));

    for (size_t i = 0; i < nl; i++) {
        array[i] = a->data[i + shift_m];
    }
    if (num % 16 != 0) {
        unsigned short mask_m = (1 << (num % 16));
        unsigned short mask_l = mask_m - 1;
        for (size_t i = 0; i < nl; i++) {
            array[i] >>= (num % 16);
            if (i + 1 < nl) {
                array[i] |= ((array[i + 1] & mask_l) << (16 - num % 16));
            }
        }
    }
    if (res->data == NULL) {
        res->data = array;
        res->max_size = res->size = nl;
    } else {
        skr_free(res->data);
        res->data = array;
        res->max_size = res->size = nl;
    }
    res->positive = a->positive;
    sub_integer_fit(res);
}
void sub_integer_and(struct sub_integer *res, const struct sub_integer *a, const struct sub_integer *b) {
    size_t l = a->size;
    if (b->size > l) l = b->size;
    sub_integer_resize(res, l);
    for (size_t i = 0; i < l; i++) {
        if (i < b->size && i < a->size) {
            res->data[i] = (a->data[i] & b->data[i]);
        } else {
            res->data[i] = 0;
        }
    }
    sub_integer_fit(res);
}
void sub_integer_xor(struct sub_integer *res, const struct sub_integer *a, const struct sub_integer *b) {
    size_t l = a->size;
    if (b->size > l) l = b->size;
    sub_integer_resize(res, l);
    for (size_t i = 0; i < l; i++) {
        if (i < b->size && i < a->size) {
            res->data[i] = (a->data[i] ^ b->data[i]);
        } else {
            if (i < b->size) res->data[i] = b->data[i];
            else if (i < a->size) res->data[i] = a->data[i];
        }
    }
    sub_integer_fit(res);
}
void sub_integer_or(struct sub_integer *res, const struct sub_integer *a, const struct sub_integer *b) {
    size_t l = a->size;
    if (b->size > l) l = b->size;
    sub_integer_resize(res, l);
    for (size_t i = 0; i < l; i++) {
        if (i < b->size && i < a->size) {
            res->data[i] = (a->data[i] | b->data[i]);
        } else {
            if (i < b->size) res->data[i] = b->data[i];
            else if (i < a->size) res->data[i] = a->data[i];
        }
    }
    sub_integer_fit(res);
}

void sub_integer_add(struct sub_integer *res, const struct sub_integer *a, const struct sub_integer *b) {
    unsigned short _positive = a->positive;
    if (a->positive ^ b->positive) {
        if (sub_integer_cmp_pos(a->data, b->data, a->size, b->size) < 0) {
            sub_integer_sub_pos(res, b->data, a->data, b->size, a->size)
            _positive = b->positive;
        } else {
            sub_integer_sub_pos(res, a->data, b->data, a->size, b->size)
        }
    } else {
        if (sub_integer_cmp_pos(a->data, b->data, a->size, b->size) < 0) {
            sub_integer_add_pos(res, b->data, a->data, b->size, a->size)
        } else {
            sub_integer_add_pos(res, a->data, b->data, a->size, b->size)
        }
    }
    res->positive = _positive;
}
void sub_integer_sub(struct sub_integer *res, const struct sub_integer *a, const struct sub_integer *b) {
    unsigned short _positive = a->positive;
    if (a->positive ^ b->positive) {
        if (sub_integer_cmp_pos(a->data, b->data, a->size, b->size) < 0) {
            sub_integer_add_pos(res, b->data, a->data, b->size, a->size)
        } else {
            sub_integer_add_pos(res, a->data, b->data, a->size, b->size)
        }
    } else {
        if (sub_integer_cmp_pos(a->data, b->data, a->size, b->size) < 0) {
            sub_integer_sub_pos(res, b->data, a->data, b->size, a->size)
            _positive = !b->positive;
        } else {
            sub_integer_sub_pos(res, a->data, b->data, a->size, b->size)
        }
    }
    res->positive = _positive;
}
void sub_integer_mul(struct sub_integer *res, const struct sub_integer *a, const struct sub_integer *b) {
    unsigned short positive = !(a->positive ^ b->positive);
    sub_integer_mul_pos(res, a->data, b->data, a->size, b->size);
    res->positive = positive;
}
void sub_integer_div(struct sub_integer *res, const struct sub_integer *a, const struct sub_integer *b) {
    if (b->size > a->size || sub_integer_cmp_pos(a->data, b->data, a->size, b->size) < 0) {
        sub_integer_resize(res, 0);
        return;
    }
    if (b->size == 1 && b->data[0] == 0) {
        sub_integer_clear(res);
    }
    struct sub_integer *x = sub_integer_new();
    struct sub_integer *r = sub_integer_new();
    size_t _i = a->size * 16;
    size_t mod;
    while (_i > 0) {
        sub_integer_ls(x, x, 1);
        sub_integer_ls(r, r, 1);
        _i--;
        mod = (_i) % 16;
        if (x->size == 0) sub_integer_resize(x, 1);
        x->data[0] |= ((a->data[_i / 16] & (1 << mod)) >> mod);
        if (sub_integer_cmp_pos(x->data, b->data, x->size, b->size) >= 0) {
            sub_integer_sub_pos(x, x->data, b->data, x->size, b->size)
            if (r->size == 0) sub_integer_resize(r, 1);
            r->data[0] |= 1;
        }
    }
    r->positive = !(a->positive ^ b->positive);
    sub_integer_set(res, r);
    sub_integer_free(x);
    sub_integer_free(r);
}
void sub_integer_mod(struct sub_integer *res, const struct sub_integer *a, const struct sub_integer *b) {
    if (b->size > a->size || sub_integer_cmp_pos(a->data, b->data, a->size, b->size) < 0) {
        sub_integer_set(res, a);
        while (!res->positive) sub_integer_add(res, res, b);
        return;
    }
    if (b->size == 1 && b->data[0] == 0) {
        sub_integer_clear(res);
    }
    struct sub_integer *x = sub_integer_new();
    size_t _i = a->size * 16;
    size_t mod;
    while (_i > 0) {
        sub_integer_ls(x, x, 1);
        _i--;
        mod = (_i) % 16;
        if (x->size == 0) sub_integer_resize(x, 1);
        x->data[0] |= ((a->data[_i / 16] & (1 << mod)) >> mod);
        sub_integer_fit(x);
        if (sub_integer_cmp_pos(x->data, b->data, x->size, b->size) >= 0) {
            sub_integer_sub_pos(x, x->data, b->data, x->size, b->size)
        }
    }
    x->positive = a->positive;
    while (!x->positive) sub_integer_add(x, x, b);
    sub_integer_set(res, x);
    sub_integer_free(x);
}
void sub_integer_inv(struct sub_integer *res, const struct sub_integer *a, const struct sub_integer *mod) {
    struct sub_integer *r_p = sub_integer_new();
    struct sub_integer *r_n = sub_integer_new();
    struct sub_integer *s_n = sub_integer_new();
    struct sub_integer *q = sub_integer_new();
    struct sub_integer *temp = sub_integer_new();

    sub_integer_set(r_p, a);
    sub_integer_set(r_n, mod);
    sub_integer_set_ui(res, 1);
    sub_integer_set_ui(s_n, 0);
    while (r_n->size > 0) {
        sub_integer_div(q, r_p, r_n);

        sub_integer_set(temp, r_n);
        sub_integer_mul(r_n, r_n, q);
        sub_integer_sub(r_n, r_p, r_n);
        sub_integer_set(r_p, temp);

        sub_integer_set(temp, s_n);
        sub_integer_mul(s_n, s_n, q);
        sub_integer_sub(s_n, res, s_n);
        sub_integer_set(res, temp);
    }
    while (!res->positive) sub_integer_add(res, res, mod);

    sub_integer_free(r_p);
    sub_integer_free(r_n);
    sub_integer_free(s_n);
    sub_integer_free(q);
    sub_integer_free(temp);
}
void sub_integer_powm(struct sub_integer *res, const struct sub_integer *a, const struct sub_integer *b, const struct sub_integer *mod) {
    struct sub_integer *a1 = sub_integer_new();
    sub_integer_set(a1, a);
    sub_integer_set_ui(res, 1);
    size_t size = b->size * 16;
    size_t _size = 0;
    for (size_t i = 0; i < size; i++)
        if (b->data[i / 16] & (1 << (i % 16))) _size = i;
    for (size_t i = 0; i <= _size; i++) {
        if (b->data[i / 16] & (1 << (i % 16))) {
            sub_integer_mul(res, res, a1);
            sub_integer_mod(res, res, mod);
        }
        sub_integer_mul(a1, a1, a1);
        sub_integer_mod(a1, a1, mod);
    }
    sub_integer_free(a1);
}


char get_char_16(unsigned code) {
    if (code <= 9) {
        return (char) (code + '0');
    }
    return (char) (code - 10 + 'a');
}
unsigned short set_char_16(char x) {
    if (x >= '0' && x <= '9') {
        return (unsigned short) (x - '0');
    }
    if (x >= 'a' && x <= 'f') {
        return (unsigned short) (x - 'a' + 10);
    }
    if (x >= 'A' && x <= 'F') {
        return (unsigned short) (x - 'A' + 10);
    }
    return (unsigned short) 0;
}

struct sub_integer *sub_integer_new() {
    struct sub_integer *res = skr_malloc(SUB_INTEGER_SIZE);
    res->data = NULL;
    res->max_size = res->size = 0;
    res->positive = 1;
    return res;
}
void sub_integer_set(struct sub_integer *res, const struct sub_integer *a) {
    if(a == NULL) return;
    sub_integer_resize(res, a->size);
    memcpy(res->data, a->data, a->size * sizeof(unsigned short));
    res->positive = a->positive;
}
void sub_integer_clear(struct sub_integer *res) {
    sub_integer_resize(res, 0);
    res->positive = 1;
}
void sub_integer_free(struct sub_integer *res) {
    if (res->data != NULL) skr_free(res->data);
    skr_free(res);
}
int sub_integer_cmp(const struct sub_integer *a, const struct sub_integer *b) {
    if (a->positive ^ b->positive) {
        if (a->positive) {
            return 1;
        }
        return -1;
    }
    int res = sub_integer_cmp_pos(a->data, b->data, a->size, b->size);
    if (!a->positive) {
        res *= -1;
    }
    return res;
}

void sub_integer_set_ui(struct sub_integer *res, unsigned number) {
    if (res == NULL) return;
    sub_integer_resize(res, 2);
    res->positive = 1;
    res->data[0] = (unsigned short) (number % 65536);
    res->data[1] = (unsigned short) (number / 65536);
    sub_integer_fit(res);
}
void sub_integer_set_si(struct sub_integer *res, signed number) {
    if (res == NULL) return;
    sub_integer_resize(res, 2);
    res->positive = 1;
    if (number < 0){
        number *= -1;
        res->positive = 0;
    }
    res->data[0] = (unsigned short) (number % 65536);
    res->data[1] = (unsigned short) (number / 65536);
    sub_integer_fit(res);
}
unsigned sub_integer_get_ui(const struct sub_integer *res) {
    if (res == NULL) return 0;
    unsigned result = 0;
    if (res->size > 1) {
        result = res->data[1];
        result <<= 16;
    }
    if (res->size > 0) result += res->data[0];
    return result;
}
signed   sub_integer_get_si(const struct sub_integer *res) {
    if (res == NULL) return 0;
    int result = 0;
    if (res->size > 1) {
        result = res->data[1];
        result <<= 16;
    }
    if (res->size > 0) result += res->data[0];
    if (!res->positive) {
        result *= -1;
    }
    return result;
}





// TLV Methods
void sub_integer_set_tlv(struct sub_integer *res, const struct string_st *tlv) {
    if(res == NULL) return;
    sub_integer_clear(res);
    if(tlv == NULL) return;
    if(tlv_get_tag(tlv->data) != SUB_INTEGER_TLV) return;

    char *data = tlv_get_value(tlv->data);
    size_t size = tlv_get_size(tlv->data);

    size_t type_size = 8;
    size_t dif = (16 / type_size);
    size_t j = 0;
    if(size > 0 && data[0] == 0x00){
        size--;
        j++;
        res->positive = 0;
    }
    size_t _size = size / dif + (size_t) (size % dif != 0);
    sub_integer_resize(res, _size);
    for (size_t i = 0; i < _size; i++) res->data[i] = 0;
    for (size_t i = size; i > 0; j++) {
        i--;
        res->data[i / dif] <<= type_size;
        res->data[i / dif] |= (unsigned char)data[j];
    }
    sub_integer_fit(res);
}
void sub_integer_get_tlv(const struct sub_integer *res, struct string_st *tlv) {
    if(tlv == NULL) return;
    string_clear(tlv);
    if(res == NULL) return;

    size_t type_size = 8;
    size_t dif = (16 / type_size);
    size_t size = (res->size * dif);

    char *data = skr_malloc(size + 2);

    size_t j = 0;
    if(!res->positive) data[j++] = 0x00;
    unsigned short temp, used = 0;
    for (size_t i = 0; i < res->size; i++) {
        for(size_t k=dif; k>0;k--){
            temp = res->data[res->size - i - 1];
            for(size_t _k = 1; _k < k;_k++) temp >>= type_size;
            temp %= (1<<type_size);
            if(temp != 0 || used != 0){
                data[j++] = (char)(unsigned char)temp;
                used = 1;
            }
        }
    }
    tlv_set_str(tlv, SUB_INTEGER_TLV, data, j);
    skr_free(data);
}
