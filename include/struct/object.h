#ifndef OBJECT_H
#define OBJECT_H

#include "object_structs.h"

// Object Class
struct object_st {
    struct object_type *type;
    size_t counter;
    int flag;
    void *data;
};
// Standard operations
struct object_st *object_new();
void object_free(struct object_st *);

void object_set(struct object_st *, const struct object_st *);
void object_copy(struct object_st *, const struct object_st *);

void object_mark(struct object_st *);
void object_unmark(struct object_st *);

void object_clear(struct object_st *);
int object_cmp(const struct object_st *, const struct object_st *);

// Pointer Methods
struct object_st *object_copy_obj(struct object_st *);
void object_set_type(struct object_st *, struct object_type *);
void object_set_pointer(struct object_st *, struct object_st *);

// TLV Methods
int object_set_tlv(struct object_st *, const struct string_st *);
void object_get_tlv(struct object_st *, struct string_st *);
int object_set_tlv_self(struct object_st *, struct object_type *);

// Sub method
struct object_st *object_subscript(struct error_st *, struct object_st *, const struct object_st *);
struct object_st *object_attrib(struct error_st *, const struct object_st *, const struct string_st *);

// Convert Methods
void object__bool(struct object_st *, struct error_st *, const struct object_st *);
void object__int(struct object_st *, struct error_st *, const struct object_st *);
void object__float(struct object_st *, struct error_st *, const struct object_st *);
void object__str(struct object_st *, struct error_st *, struct object_st *);

// Math Methods
void object__mod(struct object_st *, struct error_st *, const struct object_st *, const struct object_st *);
void object__and(struct object_st *, struct error_st *, const struct object_st *, const struct object_st *);
void object__mul(struct object_st *, struct error_st *, const struct object_st *, const struct object_st *);
void object__add(struct object_st *, struct error_st *, const struct object_st *, const struct object_st *);
void object__sub(struct object_st *, struct error_st *, const struct object_st *, const struct object_st *);
void object__div(struct object_st *, struct error_st *, const struct object_st *, const struct object_st *);
void object__xor(struct object_st *, struct error_st *, const struct object_st *, const struct object_st *);
void object__or(struct object_st *, struct error_st *, const struct object_st *, const struct object_st *);
void object__ls(struct object_st *, struct error_st *, const struct object_st *, const struct object_st *);
void object__rs(struct object_st *, struct error_st *, const struct object_st *, const struct object_st *);
void object__neg(struct object_st *, struct error_st *, const struct object_st *);

// Print
void print_obj(const struct object_st *res, int tabs);

#endif //OBJECT_H
