#include "smart.h"

struct frame_st *frame_new() {
    struct frame_st *res = malloc(sizeof(struct frame_st));
    variable_list_data_init(&res->attrib);
    list_data_init(&res->data);
    return res;
}
void frame_set(struct frame_st *res, const struct frame_st *a) {
    variable_list_set(&res->attrib, &a->attrib);
    list_set(&res->data, &a->data);
}
void frame_clear(struct frame_st *res) {
    variable_list_clear(&res->attrib);
    list_clear(&res->data);
}
void frame_free(struct frame_st *res) {
    variable_list_data_free(&res->attrib);
    list_data_free(&res->data);
    free(res);
}

void frame_mark(struct frame_st *res) {
    if (res == NULL) return;
    list_mark(&res->data);
}
void frame_unmark(struct frame_st *res) {
    if (res == NULL) return;
    list_unmark(&res->data);
}

// TLV Methods
int frame_set_tlv(struct frame_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    frame_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_FRAME) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = variable_list_set_tlv(&res->attrib, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = list_set_tlv(&res->data, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void frame_get_tlv(const struct frame_st *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    if (res == NULL) return string_clear(tlv);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    variable_list_get_tlv(&res->attrib, tlv);

    list_get_tlv(&res->data, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    tlv_set_string(tlv, TLV_FRAME, tlv);
    string_data_free(&_tlv_data);
}