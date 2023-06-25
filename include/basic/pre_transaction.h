#ifndef STRUCT_PRE_TRANSACTION_H
#define STRUCT_PRE_TRANSACTION_H

#include "struct.h"

struct pre_transaction{
    struct string_st address_from;
    struct string_st address_to;
    struct string_st currency;

    struct integer_st balance;
    struct integer_st fee;

    struct string_st private_key;
};
// Standard operations
struct pre_transaction *pre_transaction_new();
void pre_transaction_free(struct pre_transaction *);

void pre_transaction_set(struct pre_transaction *, const struct pre_transaction *);
void pre_transaction_copy(struct pre_transaction *, const struct pre_transaction *);

// Data Methods
void pre_transaction_data_init(struct pre_transaction *);
void pre_transaction_data_free(struct pre_transaction *);

void pre_transaction_clear(struct pre_transaction *);
int pre_transaction_cmp(const struct pre_transaction *, const struct pre_transaction *);


#endif //STRUCT_PRE_TRANSACTION_H
