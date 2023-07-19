#ifndef PACKET_LIST_H
#define PACKET_LIST_H

#include "struct.h"

struct packet_list_st{
    struct integer_st **packets;
    size_t size;
    size_t max_size;
};
// Standard operations
struct packet_list_st *packet_list_new();
void packet_list_free(struct packet_list_st *);

void packet_list_set(struct packet_list_st *, const struct packet_list_st *);
void packet_list_copy(struct packet_list_st *, const struct packet_list_st *);

void packet_list_clear(struct packet_list_st *);
int packet_list_cmp(const struct packet_list_st *, const struct packet_list_st *);

// Data Methods
void packet_list_data_init(struct packet_list_st *);
void packet_list_data_free(struct packet_list_st *);

// Cmp Methods
int packet_list_is_null(const struct packet_list_st *);
void packet_list_resize(struct packet_list_st *, size_t);

// TLV Methods
int packet_list_set_tlv(struct packet_list_st *, const struct string_st *);
void packet_list_get_tlv(const struct packet_list_st *, struct string_st *);

// Attrib Methods
struct object_st *packet_list_subscript(struct error_st *, struct packet_list_st *, const struct object_st *);

#endif //PACKET_LIST_H
