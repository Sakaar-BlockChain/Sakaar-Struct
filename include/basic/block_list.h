#ifndef BLOCK_LIST_H
#define BLOCK_LIST_H

#include "struct.h"

struct block_list_st{
    struct string_st **addresses;
    struct integer_st **times;
    size_t size;
    size_t max_size;
};
// Standard operations
struct block_list_st *block_list_new();
void block_list_free(struct block_list_st *);

void block_list_set(struct block_list_st *, const struct block_list_st *);
void block_list_copy(struct block_list_st *, const struct block_list_st *);

void block_list_clear(struct block_list_st *);
int block_list_cmp(const struct block_list_st *, const struct block_list_st *);

// Data Methods
void block_list_data_init(struct block_list_st *);
void block_list_data_free(struct block_list_st *);

// Cmp Methods
int block_list_is_null(const struct block_list_st *);
void block_list_resize(struct block_list_st *, size_t);
void block_list_sort(struct block_list_st *);

// TLV Methods
int block_list_set_tlv(struct block_list_st *, const struct string_st *);
void block_list_get_tlv(const struct block_list_st *, struct string_st *);

// Attrib Methods
struct object_st *block_list_subscript(struct error_st *, struct block_list_st *, const struct object_st *);

#endif //BLOCK_LIST_H
