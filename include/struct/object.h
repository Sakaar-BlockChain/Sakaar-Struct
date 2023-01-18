#ifndef OBJECT_H
#define OBJECT_H

// Container of Standard operations
#define METHOD_NEW        (void *(*)())
#define METHOD_SET        (void (*)(void *, const void *))
#define METHOD_CLEAR      (void (*)(void *))
#define METHOD_FREE       (void (*)(void *))
#define METHOD_CMP        (int (*)(const void *, const void *))

#define METHOD_GET_TLV    (void (*)(const void *, struct string_st *))
#define METHOD_SET_TLV    (void (*)(void *, const struct string_st *))

#define METHOD_MATH       (void (*)(struct object_st *, const void *, const struct object_st *))

struct string_st;
struct object_st;

struct object_op {
    void *(*_new)();
    void (*_set)(void *, const void *);
    void (*_clear)(void *);
    void (*_free)(void *);
    int (*_cmp)(const void *, const void *);
};

struct object_math_op{
    void (*_mod)(struct object_st *, const void *, const struct object_st *);
    void (*_and)(struct object_st *, const void *, const struct object_st *);
    void (*_mul)(struct object_st *, const void *, const struct object_st *);
    void (*_add)(struct object_st *, const void *, const struct object_st *);
    void (*_sub)(struct object_st *, const void *, const struct object_st *);
    void (*_div)(struct object_st *, const void *, const struct object_st *);
    void (*_xor)(struct object_st *, const void *, const struct object_st *);
    void (*_or)(struct object_st *, const void *, const struct object_st *);
    void (*_ls)(struct object_st *, const void *, const struct object_st *);
    void (*_rs)(struct object_st *, const void *, const struct object_st *);
};

struct object_type{
    struct object_op self;

    void (*_get_tlv)(const void *, struct string_st *);
    void (*_set_tlv)(void *, const struct string_st *);
    struct object_math_op *math;
};

// Object Class
struct object_st {
    struct object_type *type;
    short counter;
    void *data;

    struct object_st *class;
    struct list_st *dir;
    void *function;
};

// Standard operations
struct object_st *object_new();
void object_set(struct object_st *, const struct object_st *);
void object_clear(struct object_st *);
void object_free(struct object_st *);
int object_cmp(const struct object_st *, const struct object_st *);

// Pointer methods
struct object_st *object_copy(struct object_st *);
void object_set_type(struct object_st *, struct object_type *);
void object_set_ptr(struct object_st *res, struct object_st *data);

// TLV methods
void object_set_tlv(struct object_st *, const struct string_st *);
void object_get_tlv(const struct object_st *, struct string_st *);
void object_set_tlv_self(struct object_st *, struct object_type *);

void object__mod(struct object_st *, const struct object_st *, const struct object_st *);
void object__and(struct object_st *, const struct object_st *, const struct object_st *);
void object__mul(struct object_st *, const struct object_st *, const struct object_st *);
void object__add(struct object_st *, const struct object_st *, const struct object_st *);
void object__sub(struct object_st *, const struct object_st *, const struct object_st *);
void object__div(struct object_st *, const struct object_st *, const struct object_st *);
void object__xor(struct object_st *, const struct object_st *, const struct object_st *);
void object__or(struct object_st *, const struct object_st *, const struct object_st *);
void object__ls(struct object_st *, const struct object_st *, const struct object_st *);
void object__rs(struct object_st *, const struct object_st *, const struct object_st *);

void print_obj(const struct object_st *res, int tabs);

#endif //OBJECT_H
