#ifndef ERROR_H
#define ERROR_H

#include "struct.h"

#define ErrorType_Import    "Import Error\0"
#define ErrorType_Tokenizer "Tokenizer Error\0"
#define ErrorType_Syntax    "Syntax Error\0"
#define ErrorType_Semantic  "Semantic Error\0"
#define ErrorType_RunTime   "Semantic Error\0"
#define ErrorType_Math      "Semantic Error\0"
#define ErrorType_Convert   "Semantic Error\0"
#define ErrorType_SubScript "Semantic Error\0"

struct error_st{
    unsigned char present;
    size_t line_num, line_pos, pos;

    struct string_st msg;
    struct string_st type;
};

// Standard operations
struct error_st *error_new();
void error_free(struct error_st *);

void error_set(struct error_st *, const struct error_st *);
void error_copy(struct error_st *, const struct error_st *);

void error_clear(struct error_st *);
int error_cmp(const struct error_st *, const struct error_st *);

void error_set_pos(struct error_st *, size_t, size_t, size_t);
void error_set_msg(struct error_st *, char *, char *);

// TLV Methods
int error_set_tlv(struct string_st *, const struct string_st *);
void error_get_tlv(const struct string_st *, struct string_st *);

// Convert Methods
void error__str(struct object_st *, struct error_st *, const struct error_st *);



#endif //ERROR_H
