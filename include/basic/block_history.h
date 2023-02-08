#ifndef STRUCT_BLOCK_HISTORY_H
#define STRUCT_BLOCK_HISTORY_H

#include "struct.h"

struct block_history {
    struct list_st *transactions;
    struct integer_st *benefit;
    struct integer_st *time;
    struct string_st *address_outside;
    struct string_st *hash;
    struct string_st *smart_contract;
    struct integer_st *result;
};
// Standard operations
struct block_history *block_history_new();
void block_history_set(struct block_history *, const struct block_history *);
void block_history_clear(struct block_history *);
void block_history_free(struct block_history *);

// TLV Methods
void block_history_set_tlv(struct block_history *, const struct string_st *);
void block_history_get_tlv(const struct block_history *, struct string_st *);


#endif //STRUCT_BLOCK_HISTORY_H
