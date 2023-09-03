#ifndef OP_OBJECT_H
#define OP_OBJECT_H

#include "smart.h"

struct op_object{
    // Class Object
    struct frame_st *attr;

    // Function inside
    struct frame_st *closure;
    size_t argument;
    size_t class_body;
};

struct op_object *op_object_new();
void op_object_free(struct op_object *);

void op_object_set(struct op_object *, const struct op_object *);
void op_object_copy(struct op_object *, const struct op_object *);

void op_object_mark(struct op_object *);
void op_object_unmark(struct op_object *);

void op_object_clear(struct op_object *);
int8_t op_object_cmp(const struct op_object *, const struct op_object *);

void op_object_define(struct op_object *, struct op_class *);

// TLV Methods
int8_t op_object_set_tlv(struct op_object *, const struct string_st *);
void op_object_get_tlv(const struct op_object *, struct string_st *);

// Sub method
struct object_st *op_object_subscript(struct error_st *, struct op_object *, const struct object_st *);
struct object_st *op_object_attrib(struct error_st *, const struct op_object *, const struct string_st *);

// Convert Methods
//void op_object__bool(struct object_st *, struct object_st *, const struct op_object *);
//void op_object__int(struct object_st *, struct object_st *, const struct op_object *);
//void op_object__float(struct object_st *, struct object_st *, const struct op_object *);
//void op_object__str(struct object_st *, struct object_st *, const struct op_object *);
//
//// Math Methods
//void op_object__mod(struct object_st *, struct object_st *, const struct op_object *, const struct object_st *);
//void op_object__and(struct object_st *, struct object_st *, const struct op_object *, const struct object_st *);
//void op_object__mul(struct object_st *, struct object_st *, const struct op_object *, const struct object_st *);
//void op_object__add(struct object_st *, struct object_st *, const struct op_object *, const struct object_st *);
//void op_object__sub(struct object_st *, struct object_st *, const struct op_object *, const struct object_st *);
//void op_object__div(struct object_st *, struct object_st *, const struct op_object *, const struct object_st *);
//void op_object__xor(struct object_st *, struct object_st *, const struct op_object *, const struct object_st *);
//void op_object__or(struct object_st *, struct object_st *, const struct op_object *, const struct object_st *);
//void op_object__ls(struct object_st *, struct object_st *, const struct op_object *, const struct object_st *);
//void op_object__rs(struct object_st *, struct object_st *, const struct op_object *, const struct object_st *);
//void op_object__neg(struct object_st *, struct object_st *, const struct op_object *);


#endif //OP_OBJECT_H
