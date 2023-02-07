#ifndef SMART_PRE_TRANSACTION_H
#define SMART_PRE_TRANSACTION_H

#include "struct.h"

struct pre_transaction{
    struct string_st *address_from;
    struct string_st *address_to;
    struct string_st *currency;

    struct integer_st *balance;
    struct integer_st *fee;

    struct string_st *private_key;
};
// Standard operations
struct pre_transaction *pre_transaction_new();
void pre_transaction_set(struct pre_transaction *, const struct pre_transaction *);
void pre_transaction_clear(struct pre_transaction *);
void pre_transaction_free(struct pre_transaction *);


#endif //SMART_PRE_TRANSACTION_H
