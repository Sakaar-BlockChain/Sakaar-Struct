#ifndef FLOAT_H
#define FLOAT_H

#include "gmp.h"
#include "struct.h"

struct float_st{
    union {
        struct sub_float *data;
        mpf_t mpz_int;
    };
};
// Standard operations
struct float_st *float_new();
void float_set(struct float_st *, const struct float_st *);
void float_clear(struct float_st *);
void float_free(struct float_st *);
int float_cmp(const struct float_st *obj1, const struct float_st *obj2);
int float_is_null(const struct float_st *);

// Class methods
void float_ls(struct float_st *, const struct float_st *, size_t );
void float_rs(struct float_st *, const struct float_st *, size_t );

void float_add(struct float_st *, const struct float_st *, const struct float_st *);
void float_sub(struct float_st *, const struct float_st *, const struct float_st *);
void float_mul(struct float_st *, const struct float_st *, const struct float_st *);
void float_div(struct float_st *, const struct float_st *, const struct float_st *);
void float_neg(struct float_st *, const struct float_st *);

// Math methods
void float__mul(struct object_st *, const struct float_st *, const struct object_st *);
void float__add(struct object_st *, const struct float_st *, const struct object_st *);
void float__sub(struct object_st *, const struct float_st *, const struct object_st *);
void float__div(struct object_st *, const struct float_st *, const struct object_st *);
void float__ls(struct object_st *, const struct float_st *, const struct object_st *);
void float__rs(struct object_st *, const struct float_st *, const struct object_st *);
void float__neg(struct object_st *, const struct float_st *);

// Convert methods
void float__bool(struct object_st *res, struct float_st *obj);
void float__int(struct object_st *res, struct float_st *obj);
void float__float(struct object_st *res, struct float_st *obj);
void float__str(struct object_st *res, struct float_st *obj);

// Print
void print_float(const struct float_st *);

#endif //FLOAT_H
