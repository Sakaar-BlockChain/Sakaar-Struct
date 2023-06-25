#ifndef STRUCT_BLOCK_H
#define STRUCT_BLOCK_H

#include "struct.h"
#include "time.h"
#include "block_history.h"

struct block_st {
    struct transaction_list_st transactions;
    struct integer_st benefit;
    struct integer_st time;
    struct string_st address_outside;
    struct string_st hash;
    struct string_st smart_contract;
    struct address_list_st nodes_done;
    struct integer_st result_pros;
    struct integer_st result_cons;
    struct integer_st voted;
};
// Standard operations
struct block_st *block_new();
void block_free(struct block_st *);

void block_set(struct block_st *, const struct block_st *);
void block_copy(struct block_st *, const struct block_st *);

void block_clear(struct block_st *);
int block_cmp(const struct block_st *, const struct block_st *);

// Data Methods
void block_data_init(struct block_st *);
void block_data_free(struct block_st *);

// Cmp Methods
int block_is_null(const struct block_st *);

// TLV Methods
int block_set_tlv(struct block_st *, const struct string_st *);
void block_get_tlv(const struct block_st *, struct string_st *);

// Attrib Methods
struct object_st *block_attrib(struct error_st *, const struct block_st *, const struct string_st *);


#endif //STRUCT_BLOCK_H
