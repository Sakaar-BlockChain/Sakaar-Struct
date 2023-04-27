#ifndef STRUCT_CURRENCY_H
#define STRUCT_CURRENCY_H

#include "struct.h"

struct currency_st {
    struct string_st name;
    struct string_st smart_contract;
    struct string_st info;

    struct integer_st hash_type;
    struct integer_st crypto_type;
    struct integer_st crypto_base;

    struct integer_st our;
    struct integer_st product;
};
// Standard operations
struct currency_st *currency_new();
void currency_free(struct currency_st *);

void currency_set(struct currency_st *, const struct currency_st *);
void currency_copy(struct currency_st *, const struct currency_st *);

void currency_clear(struct currency_st *);
int currency_cmp(const struct currency_st *, const struct currency_st *);

// TLV Methods
void currency_set_tlv(struct currency_st *, const struct string_st *);
void currency_get_tlv(const struct currency_st *, struct string_st *);

// Attrib Methods
struct object_st *currency_attrib(struct object_st *, const struct currency_st *, const struct string_st *);


#endif //STRUCT_CURRENCY_H
