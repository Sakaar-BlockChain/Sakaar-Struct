#ifndef RESTORE_LIST_H
#define RESTORE_LIST_H

#include "basic.h"
#include "restore_stack.h"

struct restore_list_st {
    struct restore_list_elm_st **data;
    size_t size;
    size_t max_size;
};

struct restore_list_st *restore_list_new();
void restore_list_free(struct restore_list_st *);

void restore_list_set(struct restore_list_st *, const struct restore_list_st *);
void restore_list_copy(struct restore_list_st *, const struct restore_list_st *);

void restore_list_clear(struct restore_list_st *);
int8_t restore_list_cmp(const struct restore_list_st *, const struct restore_list_st *);

// Data Methods
void restore_list_data_init(struct restore_list_st *);
void restore_list_data_free(struct restore_list_st *);

// Cmp Methods
void restore_list_resize(struct restore_list_st *, size_t);
void restore_list_find(const struct restore_list_st *, struct restore_stack_st *, const struct hash_time_st *, size_t, size_t *, size_t *);

// TLV Methods
int8_t restore_list_set_tlv(struct restore_list_st *, const struct string_st *);
void restore_list_get_tlv(const struct restore_list_st *, struct string_st *);

#endif //RESTORE_LIST_H
