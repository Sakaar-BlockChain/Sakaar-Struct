#ifndef STRUCT_WALLET_H
#define STRUCT_WALLET_H

#include "struct.h"
#include "wallet_data.h"
#include "wallet_smart.h"

struct wallet_st {
    struct wallet_data *data;
    struct wallet_smart *smart;
};
// Standard operations
struct wallet_st *wallet_new();
void wallet_free(struct wallet_st *);

void wallet_set(struct wallet_st *, const struct wallet_st *);
void wallet_copy(struct wallet_st *, const struct wallet_st *);

void wallet_clear(struct wallet_st *);
int wallet_cmp(const struct wallet_st *, const struct wallet_st *);

// TLV Methods
int wallet_set_tlv(struct wallet_st *, const struct string_st *);
void wallet_get_tlv(const struct wallet_st *, struct string_st *);

// Attrib Methods
struct object_st *wallet_attrib(struct error_st *, const struct wallet_st *, const struct string_st *);


#endif //STRUCT_WALLET_H
