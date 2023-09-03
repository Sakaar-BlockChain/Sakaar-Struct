#ifndef STRUCT_CURRENCY_H
#define STRUCT_CURRENCY_H

#include "struct.h"

struct currency_st {
    struct string_st name;
    struct string_st smart_contract;
    struct string_st info;

    size_t hash_type;
    size_t crypto_type;
    size_t crypto_base;

    struct integer_st our;
    struct integer_st product;
};
// Standard operations
struct currency_st *currency_new();
void currency_free(struct currency_st *);

void currency_set(struct currency_st *, const struct currency_st *);
void currency_copy(struct currency_st *, const struct currency_st *);

void currency_clear(struct currency_st *);
int8_t currency_cmp(const struct currency_st *, const struct currency_st *);

// Data Methods
void currency_data_init(struct currency_st *);
void currency_data_free(struct currency_st *);

// TLV Methods
int8_t currency_set_tlv(struct currency_st *, const struct string_st *);
void currency_get_tlv(const struct currency_st *, struct string_st *);

// Attrib Methods
struct object_st *currency_attrib(struct error_st *, const struct currency_st *, const struct string_st *);


#endif //STRUCT_CURRENCY_H
