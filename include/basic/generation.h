#ifndef STRUCT_GENERATION_H
#define STRUCT_GENERATION_H

#include "struct.h"

struct generation {
    struct integer_st *time;
    struct string_st *data;
    struct string_st *hash;
};
// Standard operations
struct generation *generation_new();
void generation_free(struct generation *);

void generation_set(struct generation *, const struct generation *);
void generation_copy(struct generation *, const struct generation *);

void generation_clear(struct generation *);
int generation_cmp(const struct generation *, const struct generation *);

// TLV Methods
void generation_set_tlv(struct generation *, const struct string_st *);
void generation_get_tlv(const struct generation *, struct string_st *);

// Attrib Methods
struct object_st *generation_attrib(struct object_st *, const struct generation *, const struct string_st *);


#endif //STRUCT_GENERATION_H
