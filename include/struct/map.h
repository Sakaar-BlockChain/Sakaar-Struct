#ifndef MAP_H
#define MAP_H

#include "struct.h"

// Map Class
struct map_st{
    char *name;
    size_t size;

    struct object_st *data;
    struct object_st *next[16];
};

// Standard operations
struct map_st *map_new();
void map_set(struct map_st *, const struct map_st *);
void map_clear(struct map_st *);
void map_free(struct map_st *);
int map_cmp(const struct map_st *, const struct map_st *);
int map_is_null(const struct map_st *);

// Class methods
struct object_st *map_set_elm(struct map_st *, char *, size_t);
struct object_st *map_get_elm(struct map_st *, char *, size_t);

// TLV methods
void map_set_tlv(struct map_st *, const struct string_st *);
void map_get_tlv(const struct map_st *, struct string_st *);
void map_set_tlv_self(struct map_st *, const struct string_st *, struct object_type *);

void print_map(const struct map_st *, int tabs);

#endif //MAP_H
