#ifndef SMART_ACTIVATED_ACCOUNTS_H
#define SMART_ACTIVATED_ACCOUNTS_H

#include "struct.h"

struct activated_accounts {
    struct list_st *addresses;
};
// Standard operations
struct activated_accounts *activated_accounts_new();
void activated_accounts_set(struct activated_accounts *, const struct activated_accounts *);
void activated_accounts_clear(struct activated_accounts *);
void activated_accounts_free(struct activated_accounts *);

// TLV methods
void activated_accounts_set_tlv(struct activated_accounts *, const struct string_st *);
void activated_accounts_get_tlv(const struct activated_accounts *, struct string_st *);


#endif //SMART_ACTIVATED_ACCOUNTS_H
