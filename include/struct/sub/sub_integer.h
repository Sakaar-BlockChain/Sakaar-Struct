#ifndef SUB_INTEGER_H
#define SUB_INTEGER_H

#include "struct.h"

struct sub_integer{
    unsigned short *data, positive;
    size_t size, max_size;
};

struct sub_integer *sub_integer_new();
void sub_integer_set(struct sub_integer *, const struct sub_integer *);
void sub_integer_clear(struct sub_integer *);
void sub_integer_free(struct sub_integer *);
int sub_integer_cmp(const struct sub_integer *, const struct sub_integer *);


char get_char_16(unsigned);
unsigned short set_char_16(char );

void sub_integer_resize(struct sub_integer *, size_t);
void sub_integer_fit(struct sub_integer *);

int sub_integer_cmp_pos(const unsigned short *, const unsigned short *, size_t, size_t);
void sub_integer_add_pos(struct sub_integer *, const unsigned short *, const unsigned short *, size_t, size_t);
void sub_integer_sub_pos(struct sub_integer *, const unsigned short *, const unsigned short *, size_t, size_t);
void sub_integer_mul_pos(struct sub_integer *, const unsigned short *, const unsigned short *, unsigned, unsigned);

void sub_integer_ls(struct sub_integer *res, const struct sub_integer *a, size_t num);
void sub_integer_rs(struct sub_integer *res, const struct sub_integer *a, size_t num);
void sub_integer_and(struct sub_integer *res, const struct sub_integer *a, const struct sub_integer *b);
void sub_integer_xor(struct sub_integer *res, const struct sub_integer *a, const struct sub_integer *b);
void sub_integer_or(struct sub_integer *res, const struct sub_integer *a, const struct sub_integer *b);

void sub_integer_add(struct sub_integer *, const struct sub_integer *, const struct sub_integer *);
void sub_integer_sub(struct sub_integer *, const struct sub_integer *, const struct sub_integer *);
void sub_integer_mul(struct sub_integer *, const struct sub_integer *, const struct sub_integer *);
void sub_integer_div(struct sub_integer *, const struct sub_integer *, const struct sub_integer *);
void sub_integer_mod(struct sub_integer *, const struct sub_integer *, const struct sub_integer *);
void sub_integer_inv(struct sub_integer *, const struct sub_integer *, const struct sub_integer *);
void sub_integer_powm(struct sub_integer *, const struct sub_integer *, const struct sub_integer *, const struct sub_integer *);

void sub_integer_set_ui(struct sub_integer *, unsigned);
void sub_integer_set_si(struct sub_integer *, signed);
unsigned sub_integer_get_ui(const struct sub_integer *);
signed   sub_integer_get_si(const struct sub_integer *);

// TLV Methods
void sub_integer_set_tlv(struct sub_integer *, const struct string_st *);
void sub_integer_get_tlv(const struct sub_integer *, struct string_st *);

void print_subint(const struct sub_integer *);


#endif //SUB_INTEGER_H
