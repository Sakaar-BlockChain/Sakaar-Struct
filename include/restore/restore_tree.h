#ifndef RESTORE_TREE_H
#define RESTORE_TREE_H

#include "struct.h"
#include "restore_list.h"

struct restore_tree_st {
    struct restore_list_st nodes;
    size_t height;

    struct integer_st block_id;
    struct string_st hash;
};

struct restore_tree_st *restore_tree_new();
void restore_tree_free(struct restore_tree_st *);

void restore_tree_set(struct restore_tree_st *, const struct restore_tree_st *);
void restore_tree_copy(struct restore_tree_st *, const struct restore_tree_st *);

void restore_tree_clear(struct restore_tree_st *);
int8_t restore_tree_cmp(const struct restore_tree_st *, const struct restore_tree_st *);

// Data Methods
void restore_tree_data_init(struct restore_tree_st *);
void restore_tree_data_free(struct restore_tree_st *);

// TLV Methods
int8_t restore_tree_set_tlv(struct restore_tree_st *, const struct string_st *);
void restore_tree_get_tlv(const struct restore_tree_st *, struct string_st *);

#endif //RESTORE_TREE_H
