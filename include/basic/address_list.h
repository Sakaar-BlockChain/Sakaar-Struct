#ifndef ADDRESS_LIST_H
#define ADDRESS_LIST_H

#include "struct.h"

struct address_list_st{
    struct string_st **addresses;
    size_t size;
    size_t max_size;
};
// Standard operations
struct address_list_st *address_list_new();
void address_list_free(struct address_list_st *);

void address_list_set(struct address_list_st *, const struct address_list_st *);
void address_list_copy(struct address_list_st *, const struct address_list_st *);

void address_list_clear(struct address_list_st *);
int8_t address_list_cmp(const struct address_list_st *, const struct address_list_st *);

// Data Methods
void address_list_data_init(struct address_list_st *);
void address_list_data_free(struct address_list_st *);

// Cmp Methods
int8_t address_list_is_null(const struct address_list_st *);
void address_list_resize(struct address_list_st *, size_t);

// TLV Methods
int8_t address_list_set_tlv(struct address_list_st *, const struct string_st *);
void address_list_get_tlv(const struct address_list_st *, struct string_st *);

// Attrib Methods
struct object_st *address_list_subscript(struct error_st *, struct address_list_st *, const struct object_st *);

#endif //ADDRESS_LIST_H
