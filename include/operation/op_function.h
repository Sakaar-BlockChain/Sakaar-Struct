#ifndef OP_FUNCTION_H
#define OP_FUNCTION_H

#include "smart.h"

struct op_function {
    struct frame_st *closure;
    size_t argument;
    size_t function_body;
    size_t argument_size;

    // Function outside
    void (*func)(struct list_st *, struct object_st *, struct op_function *);
};

struct op_function *op_function_new();
void op_function_free(struct op_function *);

void op_function_set(struct op_function *, const struct op_function *);
void op_function_copy(struct op_function *, const struct op_function *);

void op_function_clear(struct op_function *);
int op_function_cmp(const struct op_function *, const struct op_function *);

void op_function_define(struct op_function *, size_t, struct parser_st *);

// TLV Methods
//int op_function_set_tlv(struct op_function *, const struct string_st *);
//void op_function_get_tlv(const struct op_function *, struct string_st *);

// Convert Methods
//void op_object__bool(struct object_st *, struct object_st *, const struct op_object *);
//void op_object__int(struct object_st *, struct object_st *, const struct op_object *);
//void op_object__float(struct object_st *, struct object_st *, const struct op_object *);
//void op_object__str(struct object_st *, struct object_st *, const struct op_object *);


#endif //OP_FUNCTION_H
