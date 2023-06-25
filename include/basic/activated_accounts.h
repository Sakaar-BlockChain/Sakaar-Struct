#ifndef STRUCT_ACTIVATED_ACCOUNTS_H
#define STRUCT_ACTIVATED_ACCOUNTS_H

#include "struct.h"

struct activated_accounts {
    struct string_st **addresses;
    struct integer_st **freeze;
    size_t size;
    size_t max_size;
};
// Standard operations
struct activated_accounts *activated_accounts_new();
void activated_accounts_free(struct activated_accounts *);

void activated_accounts_set(struct activated_accounts *, const struct activated_accounts *);
void activated_accounts_copy(struct activated_accounts *, const struct activated_accounts *);

void activated_accounts_clear(struct activated_accounts *);
int activated_accounts_cmp(const struct activated_accounts *, const struct activated_accounts *);

// Data Methods
void activated_accounts_data_init(struct activated_accounts *);
void activated_accounts_data_free(struct activated_accounts *);

// Cmp Methods
int activated_accounts_is_null(const struct activated_accounts *);
void activated_accounts_resize(struct activated_accounts *, size_t);

// TLV Methods
int activated_accounts_set_tlv(struct activated_accounts *, const struct string_st *);
void activated_accounts_get_tlv(const struct activated_accounts *, struct string_st *);

// Attrib Methods
struct object_st *activated_accounts_attrib(struct error_st *, const struct activated_accounts *, const struct string_st *);


#endif //STRUCT_ACTIVATED_ACCOUNTS_H
