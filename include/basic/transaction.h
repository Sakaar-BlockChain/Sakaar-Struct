#ifndef STRUCT_TRANSACTION_H
#define STRUCT_TRANSACTION_H

#include "struct.h"
#include "pre_transaction.h"

struct transaction_st {
    struct string_st *address_from;
    struct string_st *address_to;
    struct string_st *currency;

    struct integer_st *balance;
    struct integer_st *fee;

    struct integer_st *balance_from;
    struct string_st *hash_from;

    struct string_st *signature;
};
// Standard operations
struct transaction_st *transaction_new();
void transaction_free(struct transaction_st *);

void transaction_set(struct transaction_st *, const struct transaction_st *a);
void transaction_copy(struct transaction_st *, const struct transaction_st *a);

void transaction_clear(struct transaction_st *);
int transaction_cmp(const struct transaction_st *, const struct transaction_st *);

// TLV Methods
void transaction_set_tlv(struct transaction_st *, const struct string_st *);
void transaction_get_tlv(const struct transaction_st *, struct string_st *);

// Attrib Methods
struct object_st *transaction_attrib(struct object_st *, const struct transaction_st *, const struct string_st *);

#endif //STRUCT_TRANSACTION_H
