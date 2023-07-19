#include "basic.h"

struct object_sub packet_list_sub = {METHOD_SUBSCRIPT &packet_list_subscript};
struct object_tlv packet_list_tlv = {METHOD_GET_TLV &packet_list_get_tlv, METHOD_SET_TLV &packet_list_set_tlv};
struct object_type packet_list_type = {PACKET_LIST_OP, &packet_list_tlv, &packet_list_sub};

// Standard operations
struct packet_list_st *packet_list_new() {
    struct packet_list_st *res = malloc(sizeof(struct packet_list_st));
    res->packets = NULL;
    res->max_size = 0;
    res->size = 0;
    return res;
}
void packet_list_free(struct packet_list_st *res) {
    if (res == NULL) return;

    packet_list_resize(res, 0);
    if (res->packets != NULL) free(res->packets);
    free(res);
}

void packet_list_set(struct packet_list_st *res, const struct packet_list_st *a) {
    if (res == NULL) return;
    if (a == NULL) return packet_list_clear(res);

    packet_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) integer_set(res->packets[i], a->packets[i]);
}
void packet_list_copy(struct packet_list_st *res, const struct packet_list_st *a) {
    if (res == NULL) return;
    if (a == NULL) return packet_list_clear(res);

    packet_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) integer_copy(res->packets[i], a->packets[i]);
}

void packet_list_clear(struct packet_list_st *res) {
    if (res == NULL) return;
    packet_list_resize(res, 0);
}
int packet_list_cmp(const struct packet_list_st *obj1, const struct packet_list_st *obj2) {
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    if (obj1->size > obj2->size) return CMP_GRET;
    if (obj1->size < obj2->size) return CMP_LESS;
    int res_cmp_sub;
    for (size_t i = 0, size = obj1->size; i < size; i++) {
        res_cmp_sub = integer_cmp(obj1->packets[i], obj2->packets[i]);
        if (res_cmp_sub != CMP_EQ) return res_cmp_sub;
    }
    return CMP_EQ;
}

// Data Methods
void packet_list_data_init(struct packet_list_st *res) {
    if (res == NULL) return;
    res->packets = NULL;
    res->max_size = 0;
    res->size = 0;
}
void packet_list_data_free(struct packet_list_st *res) {
    if (res == NULL) return;
    packet_list_resize(res, 0);
    if (res->packets != NULL) free(res->packets);
}

// Cmp Methods
int packet_list_is_null(const struct packet_list_st *res) {
    return (res == NULL || res->size == 0);
}
void packet_list_resize(struct packet_list_st *res, size_t size) {
    if (res->packets == NULL && size != 0) {
        res->max_size = size;
        res->packets = malloc(sizeof(struct string_st *) * size);
        for (size_t i = 0; i < size; i++) res->packets[i] = NULL;
    } else if (res->max_size < size) {
        res->packets = realloc(res->packets, sizeof(struct string_st *) * size * 2);
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->packets[i] = NULL;
        res->max_size = size * 2;
    }
    for (size_t i = size, l = res->size; i < l; i++) {
        if (res->packets[i] != NULL) integer_free(res->packets[i]);
        res->packets[i] = NULL;
    }
    for (size_t i = res->size, l = size; i < l; i++) {
        if (res->packets[i] == NULL) res->packets[i] = integer_new();
    }
    res->size = size;
}

// TLV Methods
int packet_list_set_tlv(struct packet_list_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    packet_list_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_PACKET_LIST) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    result = tlv_get_value(tlv, &_tlv);

    for (size_t pos; _tlv.size && result == 0;) {
        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        pos = res->size;
        packet_list_resize(res, pos + 1);
        result = integer_set_tlv(res->packets[pos], &_tlv_data);
    }

    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void packet_list_get_tlv(const struct packet_list_st *packet_list, struct string_st *res) {
    if (res == NULL) return;
    string_clear(res);
    if (packet_list == NULL) return;

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    for (size_t i = 0, size = packet_list->size; i < size; i++)  {
        integer_get_tlv(packet_list->packets[i], &_tlv_data);
        string_concat(res, &_tlv_data);
    }
    tlv_set_string(res, TLV_PACKET_LIST, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *packet_list_subscript(struct error_st *err, struct packet_list_st *list, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj == NULL) {
        error_set_msg(err, ErrorType_Math, "Can not make operation with object None");
        return NULL;
    }
    struct object_st *res = NULL;
    if (obj->type != INTEGER_TYPE) {
        struct object_st *temp = object_new();
        if (temp == NULL) {
            error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
            return NULL;
        }
        object__int(temp, err, obj);

        if (err == NULL || !err->present) {
            res = object_new();
            if (res == NULL) {
                error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
                return NULL;
            }
            object_set_type(res, INTEGER_TYPE);
            integer_set(res->data, list->packets[integer_get_ui(temp->data) % list->size]);
        }
        object_free(temp);
        return res;
    }
    res = object_new();
    if (res == NULL) {
        error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
        return NULL;
    }
    object_set_type(res, INTEGER_TYPE);
    integer_set(res->data, list->packets[integer_get_ui(obj->data) % list->size]);
    return res;
}
