#include "struct.h"

struct object_tlv string_tlv = {METHOD_GET_TLV &string_get_tlv, METHOD_SET_TLV &string_set_tlv};
struct object_math_op string_math = {NULL, NULL, METHOD_MATH &string__mul, METHOD_MATH &string__add};
struct object_convert string_convert = {METHOD_CONVERT &string__bool, METHOD_CONVERT &string__int, METHOD_CONVERT &string__float, METHOD_CONVERT &string__str};
struct object_type string_type = {STRING_OP, &string_tlv, &string_math,  &string_convert};
// Standard operations
struct string_st *string_new() {
    struct string_st *res = skr_malloc(STRING_SIZE);
    res->data = NULL;
    res->mx_size = res->size = 0;
    return res;
}
void string_set(struct string_st *res, const struct string_st *a) {
    if (string_is_null(a)) return string_clear(res);
    string_resize(res, a->size);
    memcpy(res->data, a->data, a->size);
}
void string_clear(struct string_st *res) {
    string_resize(res, 0);
}
void string_free(struct string_st *res) {
    if (res->data != NULL) skr_free(res->data);
    skr_free(res);
}
int string_cmp(const struct string_st *obj1, const struct string_st *obj2) {
    if (obj1->size > obj2->size) return 1;
    if (obj1->size < obj2->size) return -1;
    return memcmp(obj1->data, obj2->data, obj1->size);
}
int string_is_null(const struct string_st *res){
    return (res == NULL || res->size == 0);
}

// Class Methods
void string_resize(struct string_st *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->mx_size = size;
        res->data = skr_malloc(size + 1);
        if (res->data != NULL) for (size_t i = 0; i < size + 1; i++) res->data[i] = 0;
    } else if (res->mx_size < size) {
        res->data = skr_realloc(res->data, size * 2 + 1);
        if (res->data != NULL) for (size_t i = res->mx_size; i < size * 2 + 1; i++) res->data[i] = 0;
        res->mx_size = size * 2;
    }
    if (res->size > size)
        for (size_t i = size; i < res->size; i++) {
            res->data[i] = 0;
        }
    res->size = size;
}
void string_set_str(struct string_st *res, const char *str, size_t size) {
    if (res == NULL) return;
    if (str == NULL) return string_clear(res);

    string_resize(res, size);
    memcpy(res->data, str, size);
}
void string_concat(struct string_st *res, const struct string_st *a) {
    if (res == NULL || string_is_null(a)) return;

    size_t _size = res->size;
    string_resize(res, res->size + a->size);
    memcpy(res->data + _size, a->data, a->size);
}

// TLV Methods
void string_set_tlv(struct string_st *res, const struct string_st *tlv) {
    if(res == NULL) return;
    if(string_is_null(tlv)) return string_clear(res);

    if(tlv_get_tag(tlv->data) != STRING_TLV) return;
    string_set_str(res, tlv_get_value(tlv->data),tlv_get_size(tlv->data));
}
void string_get_tlv(const struct string_st *res, struct string_st *tlv) {
    if(tlv == NULL) return;
    if(res == NULL) return string_clear(tlv);

    tlv_set_string(tlv, STRING_TLV, res);
}

// Math Methods
void string__mul(struct object_st *res, const struct string_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, STRING_TYPE);
    unsigned int count = integer_get_ui(obj2->data);
    for (unsigned int i = 0; i < count; i++)
        string_concat(res->data, obj1);
}
void string__add(struct object_st *res, const struct string_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != STRING_TYPE) return;
    object_set_type(res, STRING_TYPE);
    string_set(res->data, obj1);
    string_concat(res->data, obj2->data);
}

// Convert Methods
void string__bool(struct object_st *res, const struct string_st *obj){
    object_set_type(res, INTEGER_TYPE);
    if(obj->size == 0) integer_set_ui(res->data, 0);
    else integer_set_ui(res->data, 1);
}
void string__int(struct object_st *res, const struct string_st *obj){
    // TODO
}
void string__float(struct object_st *res, const struct string_st *obj){
    // TODO
}
void string__str(struct object_st *res, const struct string_st *obj){
    object_set_type(res, STRING_TYPE);
    string_set(res->data, obj);
}
