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
void integer_set(struct integer_st *, const struct integer_st *);
void integer_clear(struct integer_st *);
void integer_free(struct integer_st *);
int integer_cmp(const struct integer_st *obj1, const struct integer_st *obj2);

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

// Hex Methods
void _integer_set_str(struct integer_st *, const char *, size_t);
void integer_set_str(struct integer_st *, const struct string_st *);
void integer_get_str(const struct integer_st *, struct string_st *);
void integer_set_time(struct integer_st *);

void _integer_set_str_dec(struct integer_st *, const char *, size_t);
void _integer_set_str_oct(struct integer_st *, const char *, size_t);
void _integer_set_str_bin(struct integer_st *, const char *, size_t);
// Cmp Methods
int integer_is_null(const struct integer_st *);
int integer_is_neg(const struct integer_st *);
void integer_random(struct integer_st *, const struct integer_st *);

// TLV Methods
void _integer_set_tlv(struct integer_st *, const struct string_st *);
void _integer_get_tlv(const struct integer_st *, struct string_st *, unsigned);
void integer_set_tlv(struct integer_st *, const struct string_st *);
void integer_get_tlv(const struct integer_st *, struct string_st *);

// Math Methods
void integer__mod(struct object_st *, const struct integer_st *, const struct object_st *);
void integer__and(struct object_st *, const struct integer_st *, const struct object_st *);
void integer__mul(struct object_st *, const struct integer_st *, const struct object_st *);
void integer__add(struct object_st *, const struct integer_st *, const struct object_st *);
void integer__sub(struct object_st *, const struct integer_st *, const struct object_st *);
void integer__div(struct object_st *, const struct integer_st *, const struct object_st *);
void integer__xor(struct object_st *, const struct integer_st *, const struct object_st *);
void integer__or(struct object_st *, const struct integer_st *, const struct object_st *);
void integer__ls(struct object_st *, const struct integer_st *, const struct object_st *);
void integer__rs(struct object_st *, const struct integer_st *, const struct object_st *);
void integer__neg(struct object_st *, const struct integer_st *);

// Convert Methods
void integer__bool(struct object_st *res, struct integer_st *obj);
void integer__int(struct object_st *res, struct integer_st *obj);
void integer__float(struct object_st *res, struct integer_st *obj);
void integer__str(struct object_st *res, struct integer_st *obj);

// Print
void print_int(const struct integer_st *);

#endif //INTEGER_H
