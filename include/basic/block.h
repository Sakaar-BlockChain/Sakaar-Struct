#ifndef SMART_BLOCK_H
#define SMART_BLOCK_H

#include "struct.h"
#include "time.h"
#include "block_history.h"

struct block_st {
    struct list_st *transactions;
    struct integer_st *benefit;
    struct integer_st *time;
    struct string_st *address_outside;
    struct string_st *hash;
    struct string_st *smart_contract;
    struct list_st *nodes_done;
    struct integer_st *result_pros;
    struct integer_st *result_cons;
    struct integer_st *voted;
};
// Standard operations
struct block_st *block_new();
void block_set(struct block_st *, const struct block_st *);
void block_clear(struct block_st *);
void block_free(struct block_st *);
int block_is_null(const struct block_st *);

// TLV methods
void block_set_tlv(struct block_st *, const struct string_st *);
void block_get_tlv(const struct block_st *, struct string_st *);


#endif //SMART_BLOCK_H
