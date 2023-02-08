#include "basic.h"

struct object_tlv generation_tlv = {METHOD_GET_TLV &generation_get_tlv, METHOD_SET_TLV &generation_set_tlv};
struct object_type generation_type = {GENERATION_OP, &generation_tlv};
// Standard operations
struct generation *generation_new() {
    struct generation *res = skr_malloc(GENERATION_SIZE);
    res->time = integer_new();
    res->hash = string_new();
    res->data = string_new();

    integer_set_time(res->time);
    return res;
}
void generation_set(struct generation *res, const struct generation *a) {
    if (a == NULL) return generation_clear(res);

    integer_set(res->time, a->time);
    string_set(res->hash, a->hash);
    string_set(res->data, a->data);
}
void generation_clear(struct generation *res) {
    integer_clear(res->time);
    string_clear(res->hash);
    string_clear(res->data);
}
void generation_free(struct generation *res) {
    integer_free(res->time);
    string_free(res->hash);
    string_free(res->data);
    skr_free(res);
}

// TLV Methods
void generation_set_tlv(struct generation *res, const struct string_st *tlv) {
    if (res == NULL) return;
    generation_clear(res);
    if (string_is_null(tlv) || tlv_get_tag(tlv->data) != TLV_GENERATION) return;

    char *data = tlv_get_value(tlv->data);
    struct string_st *_tlv = string_new();

    data = tlv_get_next_tlv(data, _tlv);
    integer_set_tlv(res->time, _tlv);

    data = tlv_get_next_tlv(data, _tlv);
    {
        struct integer_st *num = integer_new();
        integer_set_tlv(num, _tlv);
        integer_get_str(num, res->hash);
        integer_free(num);
    }

    tlv_get_next_tlv(data, _tlv);
    string_set_tlv(res->data, _tlv);

    string_free(_tlv);
}
void generation_get_tlv(const struct generation *gen, struct string_st *res) {
    if (res == NULL) return;
    if (gen == NULL) return string_clear(res);

    struct string_st *tlv = string_new();
    integer_get_tlv(gen->time, res);

    {
        struct integer_st *num = integer_new();
        integer_set_str(num, gen->hash);
        integer_get_tlv(num, tlv);
        integer_free(num);
    }
    string_concat(res, tlv);

    string_get_tlv(gen->data, tlv);
    string_concat(res, tlv);

    tlv_set_string(res, TLV_GENERATION, res);
    string_free(tlv);
}







