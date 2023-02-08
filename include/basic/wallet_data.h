#ifndef STRUCT_WALLET_DATA_H
#define STRUCT_WALLET_DATA_H

#include "struct.h"

struct wallet_data {
    struct string_st *address;
    struct string_st *currency;
    struct string_st *address_outside;

    struct string_st *hash;
    struct string_st *pre_hash;

    struct integer_st *balance;
    struct integer_st *pre_balance;
};
// Standard operations
struct wallet_data *wallet_data_new();
void wallet_data_set(struct wallet_data *, const struct wallet_data *a);
void wallet_data_clear(struct wallet_data *);
void wallet_data_free(struct wallet_data *);

// TLV Methods
void wallet_data_set_tlv(struct wallet_data *, const struct string_st *);
void wallet_data_get_tlv(const struct wallet_data *, struct string_st *);


#endif //STRUCT_WALLET_DATA_H
