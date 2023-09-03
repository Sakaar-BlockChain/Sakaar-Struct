#ifndef HASH_TIME_H
#define HASH_TIME_H

#include "struct.h"

struct hash_time_st{
    struct string_st hash;
    struct integer_st time;
};
// Standard operations
struct hash_time_st *hash_time_new();
void hash_time_free(struct hash_time_st *);

void hash_time_set(struct hash_time_st *, const struct hash_time_st *);
void hash_time_copy(struct hash_time_st *, const struct hash_time_st *);

void hash_time_clear(struct hash_time_st *);
int8_t hash_time_cmp(const struct hash_time_st *, const struct hash_time_st *);

// Cmp Methods
int8_t hash_time_is_null(const struct hash_time_st *);
int8_t hash_time_get_string(const struct hash_time_st *, struct string_st *);

// Data Methods
void hash_time_data_init(struct hash_time_st *);
void hash_time_data_free(struct hash_time_st *);

// TLV Methods
int8_t hash_time_set_tlv(struct hash_time_st *, const struct string_st *);
void hash_time_get_tlv(const struct hash_time_st *, struct string_st *);

// Attrib Methods
struct object_st *hash_time_attrib(struct error_st *, const struct hash_time_st *, const struct string_st *);

void print_hash_time(const struct hash_time_st *, int);

#endif //HASH_TIME_H
