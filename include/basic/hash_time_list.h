#ifndef HASH_TIME_LIST_H
#define HASH_TIME_LIST_H

#include "struct.h"

struct hash_time_list_st{
    struct hash_time_st **data;
    size_t size;
    size_t max_size;
};
// Standard operations
struct hash_time_list_st *hash_time_list_new();
void hash_time_list_free(struct hash_time_list_st *);

void hash_time_list_set(struct hash_time_list_st *, const struct hash_time_list_st *);
void hash_time_list_copy(struct hash_time_list_st *, const struct hash_time_list_st *);

void hash_time_list_clear(struct hash_time_list_st *);
int8_t hash_time_list_cmp(const struct hash_time_list_st *, const struct hash_time_list_st *);

// Data Methods
void hash_time_list_data_init(struct hash_time_list_st *);
void hash_time_list_data_free(struct hash_time_list_st *);

// Cmp Methods
int8_t hash_time_list_is_null(const struct hash_time_list_st *);
void hash_time_list_resize(struct hash_time_list_st *, size_t);
void hash_time_list_sort(struct hash_time_list_st *);

// TLV Methods
int8_t hash_time_list_set_tlv(struct hash_time_list_st *, const struct string_st *);
void hash_time_list_get_tlv(const struct hash_time_list_st *, struct string_st *);

// Attrib Methods
struct object_st *hash_time_list_subscript(struct error_st *, struct hash_time_list_st *, const struct object_st *);

#endif //HASH_TIME_LIST_H
