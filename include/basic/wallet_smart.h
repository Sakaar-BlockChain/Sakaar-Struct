#ifndef STRUCT_WALLET_SMART_H
#define STRUCT_WALLET_SMART_H

#include "struct.h"
#include "currency.h"

struct wallet_smart {
    struct string_st address;
    struct string_st currency;
    struct string_st private_key;
    struct string_st smart_private;
    struct string_st smart_contract;
    struct string_st name;
    struct string_st owner;

    struct integer_st freeze;
};
// Standard operations
struct wallet_smart *wallet_smart_new();
void wallet_smart_free(struct wallet_smart *);

void wallet_smart_set(struct wallet_smart *, const struct wallet_smart *);
void wallet_smart_copy(struct wallet_smart *, const struct wallet_smart *);

void wallet_smart_clear(struct wallet_smart *);
int wallet_smart_cmp(const struct wallet_smart *, const struct wallet_smart *);

// TLV Methods
int wallet_smart_set_tlv(struct wallet_smart *, const struct string_st *);
void wallet_smart_get_tlv(const struct wallet_smart *, struct string_st *);

// Attrib Methods
struct object_st *wallet_smart_attrib(struct error_st *, const struct wallet_smart *, const struct string_st *);


#endif //STRUCT_WALLET_SMART_H
