#include "struct.h"

//struct object_tlv error_tlv = {METHOD_GET_TLV &error_get_tlv, METHOD_SET_TLV &error_set_tlv};
struct object_convert error_convert = {NULL, NULL, NULL, METHOD_CONVERT &error__str};
//struct object_type error_type = {ERROR_OP, &error_tlv,  NULL, &error_convert};
struct object_type error_type = {ERROR_OP, NULL,  NULL, &error_convert};

struct error_st *error_new() {
    struct error_st *res = malloc(sizeof(struct error_st));
    res->present = 0;
    res->line_num = 0;
    res->line_pos = 0;
    res->pos = 0;

    string_data_init(&res->type);
    string_data_init(&res->msg);
    return res;
}
void error_free(struct error_st *res) {
    string_data_free(&res->type);
    string_data_free(&res->msg);
    free(res);
}

void error_set(struct error_st *res, const struct error_st *a) {
    res->present = a->present;
    res->line_num = a->line_num;
    res->line_pos = a->line_pos;
    res->pos = a->pos;

    string_set(&res->type, &a->type);
    string_set(&res->msg, &a->msg);
}
void error_copy(struct error_st *res, const struct error_st *a) {
    res->present = a->present;
    res->line_num = a->line_num;
    res->line_pos = a->line_pos;
    res->pos = a->pos;

    string_copy(&res->type, &a->type);
    string_copy(&res->msg, &a->msg);
}

void error_clear(struct error_st *res) {
    res->present = 0;
    res->line_num = 0;
    res->line_pos = 0;
    res->pos = 0;

    string_clear(&res->type);
    string_clear(&res->msg);
}
int8_t error_cmp(const struct error_st *obj1, const struct error_st *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(&obj1->type, &obj2->type) || string_cmp(&obj1->msg, &obj2->msg)) return CMP_NEQ;
    return CMP_EQ;
}

void error_set_pos(struct error_st *res, size_t line_num, size_t line_pos, size_t pos) {
    res->line_num = line_num;
    res->line_pos = line_pos;
    res->pos = pos;
}
void error_set_msg(struct error_st *res, char *type, char *msg) {
    if (res == NULL) return;
    res->present = 1;
    string_set_str(&res->type, type, strlen(type));
    string_set_str(&res->msg, msg, strlen(msg));
}

void error__str(struct object_st *res, struct error_st *err, const struct error_st *obj1) {
    object_set_type(res, STRING_TYPE);
    string_set(res->data, &obj1->msg);
}