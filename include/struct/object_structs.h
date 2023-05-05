#ifndef OBJECT_STRUCTS_H
#define OBJECT_STRUCTS_H


struct string_st;
struct object_st;


#define METHOD_NEW        (void *(*)())
#define METHOD_FREE       (void (*)(void *))
#define METHOD_SET        (void (*)(void *, const void *))
#define METHOD_CLEAR      (void (*)(void *))
#define METHOD_CMP        (int (*)(const void *, const void *))

struct object_op {
    void *(*_new)();
    void (*_free)(void *);

    void (*_set)(void *, const void *);
    void (*_copy)(void *, const void *);

    void (*_clear)(void *);
    int (*_cmp)(const void *, const void *);
};


#define METHOD_GET_TLV    (void (*)(const void *, struct string_st *))
#define METHOD_SET_TLV    (int (*)(void *, const struct string_st *))

struct object_tlv {
    void (*_get_tlv)(const void *, struct string_st *);
    int (*_set_tlv)(void *, const struct string_st *);
};


#define METHOD_SUBSCRIPT  (struct object_st *(*)(struct object_st *, void *, const struct object_st *))
#define METHOD_ATTRIB (struct object_st *(*)(struct object_st *, void *, const struct string_st *))

struct object_sub {
    struct object_st *(*_subscript)(struct object_st *, void *, const struct object_st *);
    struct object_st *(*_attrib)(struct object_st *, void *, const struct string_st *);
};


#define METHOD_MATH       (void (*)(struct object_st *, struct object_st *, const void *, const struct object_st *))

struct object_math_op {
    void (*_mod)(struct object_st *, struct object_st *, const void *, const struct object_st *);
    void (*_and)(struct object_st *, struct object_st *, const void *, const struct object_st *);
    void (*_mul)(struct object_st *, struct object_st *, const void *, const struct object_st *);
    void (*_add)(struct object_st *, struct object_st *, const void *, const struct object_st *);
    void (*_sub)(struct object_st *, struct object_st *, const void *, const struct object_st *);
    void (*_div)(struct object_st *, struct object_st *, const void *, const struct object_st *);
    void (*_xor)(struct object_st *, struct object_st *, const void *, const struct object_st *);
    void (*_or)(struct object_st *, struct object_st *, const void *, const struct object_st *);
    void (*_ls)(struct object_st *, struct object_st *, const void *, const struct object_st *);
    void (*_rs)(struct object_st *, struct object_st *, const void *, const struct object_st *);
    void (*_ne)(struct object_st *, struct object_st *, const void *);
};


#define METHOD_CONVERT    (void (*)(struct object_st *, struct object_st *, const void *))

struct object_convert {
    void (*_bool)(struct object_st *, struct object_st *, const void *);
    void (*_int)(struct object_st *, struct object_st *, const void *);
    void (*_float)(struct object_st *, struct object_st *, const void *);
    void (*_str)(struct object_st *, struct object_st *, const void *);
};



struct object_type {
    struct object_op self;
    struct object_tlv *tlv;
    struct object_sub *sub;
    struct object_convert *convert;
    struct object_math_op *math;
};


#endif //OBJECT_STRUCTS_H
