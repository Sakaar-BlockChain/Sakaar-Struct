#ifndef STRUCT_BLOCK_HISTORY_H
#define STRUCT_BLOCK_HISTORY_H

#include "struct.h"
#include "transaction_list.h"
#include "hash_time.h"

struct block_history {
    struct hash_time_st hash_time;
    struct transaction_list_st transactions;
    struct integer_st benefit;
    struct string_st address_outside;
    struct string_st smart_contract;
    int8_t result;
};
// Standard operations
struct block_history *block_history_new();
void block_history_free(struct block_history *);

void block_history_set(struct block_history *, const struct block_history *);
void block_history_copy(struct block_history *, const struct block_history *);

void block_history_clear(struct block_history *);
int8_t block_history_cmp(const struct block_history *, const struct block_history *);

// Data Methods
void block_history_data_init(struct block_history *);
void block_history_data_free(struct block_history *);

// TLV Methods
int8_t block_history_set_tlv(struct block_history *, const struct string_st *);
void block_history_get_tlv(const struct block_history *, struct string_st *);

// Attrib Methods
struct object_st *block_history_attrib(struct error_st *, const struct block_history *, const struct string_st *);


#endif //STRUCT_BLOCK_HISTORY_H
