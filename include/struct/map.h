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

// Class Methods
struct object_st *map_set_elm(struct map_st *, char *, size_t);
struct object_st *map_get_elm(struct map_st *, char *, size_t);

void print_map(const struct map_st *, int tabs);

#endif //MAP_H
