#ifndef STRUCT_ACCOUNT_CONNECTIONS_H
#define STRUCT_ACCOUNT_CONNECTIONS_H

#include "struct.h"
#include "address_list.h"

struct account_connections {
    struct string_st address;
    struct string_st currency;
    struct address_list_st addresses;
};
// Standard operations
struct account_connections *account_connections_new();
void account_connections_free(struct account_connections *);

void account_connections_set(struct account_connections *, const struct account_connections *);
void account_connections_copy(struct account_connections *, const struct account_connections *);

void account_connections_clear(struct account_connections *);
int8_t account_connections_cmp(const struct account_connections *, const struct account_connections *);

// Data Methods
void account_connections_data_init(struct account_connections *);
void account_connections_data_free(struct account_connections *);

// TLV Methods
int8_t account_connections_set_tlv(struct account_connections *, const struct string_st *);
void account_connections_get_tlv(const struct account_connections *, struct string_st *);

// Attrib Methods
struct object_st *account_connections_attrib(struct error_st *, const struct account_connections *, const struct string_st *);


#endif //STRUCT_ACCOUNT_CONNECTIONS_H
