#ifndef SMART_GENERATION_H
#define SMART_GENERATION_H

#include "struct.h"

struct generation {
    struct integer_st *time;
    struct string_st *data;
    struct string_st *hash;
};
// Standard operations
struct generation *generation_new();
void generation_set(struct generation *, const struct generation *);
void generation_clear(struct generation *);
void generation_free(struct generation *);

// TLV methods
void generation_set_tlv(struct generation *, const struct string_st *);
void generation_get_tlv(const struct generation *, struct string_st *);


#endif //SMART_GENERATION_H
