#ifndef FRAME_H
#define FRAME_H

#include "struct.h"
#include "variable_list.h"

struct frame_st {
    struct variable_list_st attrib;
    struct list_st data;
};

struct frame_st *frame_new();
void frame_set(struct frame_st *, const struct frame_st *);
void frame_clear(struct frame_st *);
void frame_free(struct frame_st *);

void frame_mark(struct frame_st *);
void frame_unmark(struct frame_st *);

// TLV Methods
int8_t frame_set_tlv(struct frame_st *, const struct string_st *);
void frame_get_tlv(const struct frame_st *, struct string_st *);

void print_frame(const struct frame_st *, int);

#endif //FRAME_H
