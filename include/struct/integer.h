#ifndef INTEGER_H
#define INTEGER_H

#include "gmp.h"
#include "struct.h"

struct integer_st{
    union {
        struct sub_integer *data;
        mpz_t mpz_int;
    };
};
// Standard operations
struct integer_st *integer_new();
void integer_free(struct integer_st *);

void integer_set(struct integer_st *, const struct integer_st *);
void integer_copy(struct integer_st *, const struct integer_st *);

void integer_clear(struct integer_st *);
int8_t integer_cmp(const struct integer_st *, const struct integer_st *);

// Cmp Methods
int8_t integer_is_null(const struct integer_st *);
int8_t integer_is_neg(const struct integer_st *);
void integer_random(struct integer_st *, const struct integer_st *);

// Data Methods
void integer_data_init(struct integer_st *);
void integer_data_free(struct integer_st *);

// Class Methods
void integer_ls(struct integer_st *, const struct integer_st *, size_t );
void integer_rs(struct integer_st *, const struct integer_st *, size_t );
void integer_and(struct integer_st *, const struct integer_st *, const struct integer_st *);
void integer_xor(struct integer_st *, const struct integer_st *, const struct integer_st *);
void integer_or(struct integer_st *, const struct integer_st *, const struct integer_st *);

void integer_add(struct integer_st *, const struct integer_st *, const struct integer_st *);
void integer_sub(struct integer_st *, const struct integer_st *, const struct integer_st *);
void integer_mul(struct integer_st *, const struct integer_st *, const struct integer_st *);
void integer_div(struct integer_st *, const struct integer_st *, const struct integer_st *);
void integer_mod(struct integer_st *, const struct integer_st *, const struct integer_st *);
void integer_inv(struct integer_st *, const struct integer_st *, const struct integer_st *);
void integer_powm(struct integer_st *, const struct integer_st *, const struct integer_st *, const struct integer_st *);
void integer_neg(struct integer_st *, const struct integer_st *);

// Data init Methods
void integer_set_ui(struct integer_st *, unsigned);
unsigned integer_get_ui(const struct integer_st *);
signed integer_get_si(const struct integer_st *);

// String Methods
void integer_set_str_(struct integer_st *, const char *, size_t);
void integer_set_str(struct integer_st *, const struct string_st *);
void integer_get_str(const struct integer_st *, struct string_st *);
void size_get_str(size_t res, struct string_st *str);
void integer_set_time(struct integer_st *);

void integer_set_str_dec_(struct integer_st *, const char *, size_t);
void integer_set_str_oct_(struct integer_st *, const char *, size_t);
void integer_set_str_bin_(struct integer_st *, const char *, size_t);

void integer_set_dec(struct integer_st *, const struct string_st *);
void integer_get_dec(const struct integer_st *, struct string_st *);

// TLV Methods
int8_t integer_set_tlv_(struct integer_st *, const struct string_st *);
void integer_get_tlv_(const struct integer_st *, struct string_st *, unsigned);
int8_t integer_set_tlv(struct integer_st *, const struct string_st *);
void integer_get_tlv(const struct integer_st *, struct string_st *);
int8_t size_set_tlv(size_t *, const struct string_st *);
void size_get_tlv(size_t , struct string_st *);

// Math Methods
void integer__mod(struct object_st *, struct error_st *, const struct integer_st *, const struct object_st *);
void integer__and(struct object_st *, struct error_st *, const struct integer_st *, const struct object_st *);
void integer__mul(struct object_st *, struct error_st *, const struct integer_st *, const struct object_st *);
void integer__add(struct object_st *, struct error_st *, const struct integer_st *, const struct object_st *);
void integer__sub(struct object_st *, struct error_st *, const struct integer_st *, const struct object_st *);
void integer__div(struct object_st *, struct error_st *, const struct integer_st *, const struct object_st *);
void integer__xor(struct object_st *, struct error_st *, const struct integer_st *, const struct object_st *);
void integer__or(struct object_st *, struct error_st *, const struct integer_st *, const struct object_st *);
void integer__ls(struct object_st *, struct error_st *, const struct integer_st *, const struct object_st *);
void integer__rs(struct object_st *, struct error_st *, const struct integer_st *, const struct object_st *);
void integer__neg(struct object_st *, struct error_st *, const struct integer_st *);

// Convert Methods
void integer__bool(struct object_st *, struct error_st *res, const struct integer_st *obj);
void integer__int(struct object_st *, struct error_st *res, const struct integer_st *obj);
void integer__float(struct object_st *, struct error_st *res, const struct integer_st *obj);
void integer__str(struct object_st *, struct error_st *res, const struct integer_st *obj);

// Print
void print_int(const struct integer_st *);

char get_char_16(unsigned);
unsigned short set_char_16(char );

#endif //INTEGER_H
