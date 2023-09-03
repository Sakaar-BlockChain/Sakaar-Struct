#ifndef STRUCT_GENERATION_H
#define STRUCT_GENERATION_H

#include "struct.h"

struct generation_st {
    struct hash_time_st hash_time;
    struct string_st data;
};
// Standard operations
struct generation_st *generation_new();
void generation_free(struct generation_st *);

void generation_set(struct generation_st *, const struct generation_st *);
void generation_copy(struct generation_st *, const struct generation_st *);

void generation_clear(struct generation_st *);
int8_t generation_cmp(const struct generation_st *, const struct generation_st *);

// Data Methods
void generation_data_init(struct generation_st *);
void generation_data_free(struct generation_st *);

// TLV Methods
int8_t generation_set_tlv(struct generation_st *, const struct string_st *);
void generation_get_tlv(const struct generation_st *, struct string_st *);

// Attrib Methods
struct object_st *generation_attrib(struct error_st *, const struct generation_st *, const struct string_st *);


#endif //STRUCT_GENERATION_H
