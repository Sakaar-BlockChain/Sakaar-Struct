#ifndef RESTORE_BLOCK_H
#define RESTORE_BLOCK_H

#include "struct.h"
#include "restore_list.h"

struct restore_block_st {
    struct restore_list_st nodes;

    struct integer_st block_id;
    struct string_st hash;
};

struct restore_block_st *restore_block_new();
void restore_block_free(struct restore_block_st *);

void restore_block_set(struct restore_block_st *, const struct restore_block_st *);
void restore_block_copy(struct restore_block_st *, const struct restore_block_st *);

void restore_block_clear(struct restore_block_st *);
int8_t restore_block_cmp(const struct restore_block_st *, const struct restore_block_st *);

// Data Methods
void restore_block_data_init(struct restore_block_st *);
void restore_block_data_free(struct restore_block_st *);

// TLV Methods
int8_t restore_block_set_tlv(struct restore_block_st *, const struct string_st *);
void restore_block_get_tlv(const struct restore_block_st *, struct string_st *);


#endif //RESTORE_BLOCK_H
