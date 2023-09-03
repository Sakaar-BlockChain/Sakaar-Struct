#include "basic.h"

struct object_sub hash_time_list_sub = {METHOD_SUBSCRIPT &hash_time_list_subscript};
struct object_tlv hash_time_list_tlv = {METHOD_GET_TLV &hash_time_list_get_tlv, METHOD_SET_TLV &hash_time_list_set_tlv};
struct object_type hash_time_list_type = {HASH_TIME_LIST_OP, &hash_time_list_tlv, &hash_time_list_sub};

// Standard operations
struct hash_time_list_st *hash_time_list_new() {
    struct hash_time_list_st *res = malloc(sizeof(struct hash_time_list_st));
    res->data = NULL;
    res->max_size = 0;
    res->size = 0;
    return res;
}
void hash_time_list_free(struct hash_time_list_st *res) {
    if (res == NULL) return;

    hash_time_list_resize(res, 0);
    if (res->data != NULL) free(res->data);
    free(res);
}

void hash_time_list_set(struct hash_time_list_st *res, const struct hash_time_list_st *a) {
    if (res == NULL) return;
    if (a == NULL) return hash_time_list_clear(res);

    hash_time_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) hash_time_set(res->data[i], a->data[i]);
}
void hash_time_list_copy(struct hash_time_list_st *res, const struct hash_time_list_st *a) {
    if (res == NULL) return;
    if (a == NULL) return hash_time_list_clear(res);

    hash_time_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) hash_time_copy(res->data[i], a->data[i]);
}

void hash_time_list_clear(struct hash_time_list_st *res) {
    if (res == NULL) return;
    hash_time_list_resize(res, 0);
}
int8_t hash_time_list_cmp(const struct hash_time_list_st *obj1, const struct hash_time_list_st *obj2) {
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    if (obj1->size > obj2->size) return CMP_GRET;
    if (obj1->size < obj2->size) return CMP_LESS;
    int8_t res_cmp_sub;
    for (size_t i = 0, size = obj1->size; i < size; i++) {
        res_cmp_sub = hash_time_cmp(obj1->data[i], obj2->data[i]);
        if (res_cmp_sub != CMP_EQ) return res_cmp_sub;
    }
    return CMP_EQ;
}

// Data Methods
void hash_time_list_data_init(struct hash_time_list_st *res) {
    if (res == NULL) return;
    res->data = NULL;
    res->max_size = 0;
    res->size = 0;
}
void hash_time_list_data_free(struct hash_time_list_st *res) {
    if (res == NULL) return;
    hash_time_list_resize(res, 0);
    if (res->data != NULL) free(res->data);
}

// Cmp Methods
int8_t hash_time_list_is_null(const struct hash_time_list_st *res) {
    return (int8_t) (res == NULL || res->size == 0);
}
void hash_time_list_resize(struct hash_time_list_st *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->max_size = size;
        res->data = malloc(sizeof(struct hash_time_st *) * size);
        for (size_t i = 0; i < size; i++) res->data[i] = NULL;
    } else if (res->max_size < size) {
        res->data = realloc(res->data, sizeof(struct hash_time_st *) * size * 2);
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->data[i] = NULL;
        res->max_size = size * 2;
    }
    for (size_t i = size, l = res->size; i < l; i++) {
        if (res->data[i] != NULL) hash_time_free(res->data[i]);
        res->data[i] = NULL;
    }
    for (size_t i = res->size, l = size; i < l; i++) {
        if (res->data[i] == NULL) res->data[i] = hash_time_new();
    }
    res->size = size;
}
void hash_time_list_sort_merge(size_t st1, size_t fn1, size_t st2, size_t fn2, struct hash_time_list_st *data, struct hash_time_list_st *temp) {
    size_t st = st1;
    size_t pos = st1;
    while (st1 < fn1 || st2 < fn2) {
        if (st1 == fn1) {
            hash_time_set(temp->data[pos++], data->data[st2++]);
        } else if (st2 == fn2) {
            hash_time_set(temp->data[pos++], data->data[st1++]);
        } else {
            if (hash_time_cmp(data->data[st1], data->data[st2]) <= 0) {
                hash_time_set(temp->data[pos++], data->data[st1++]);
            } else {
                hash_time_set(temp->data[pos++], data->data[st2++]);
            }
        }
    }
    for (; st < fn2; st++) {
        hash_time_set(data->data[st], temp->data[st]);
    }
}
void hash_time_list_sort_split(size_t st, size_t fn, struct hash_time_list_st *data, struct hash_time_list_st *temp) {
    if (st + 1 >= fn) return;
    size_t mid = (st + fn) / 2;
    hash_time_list_sort_split(st, mid, data, temp);
    hash_time_list_sort_split(mid, fn, data, temp);
    hash_time_list_sort_merge(st, mid, mid, fn, data, temp);
}
void hash_time_list_sort(struct hash_time_list_st *res) {
    if (res == NULL) return;

    struct hash_time_list_st temp;
    hash_time_list_data_init(&temp);

    hash_time_list_resize(&temp, res->size);
    hash_time_list_sort_split(0, res->size, res, &temp);

    hash_time_list_data_free(&temp);
}

// TLV Methods
int8_t hash_time_list_set_tlv(struct hash_time_list_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    hash_time_list_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_HASH_TIME_LIST) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    result = tlv_get_value(tlv, &_tlv);

    for (size_t pos; _tlv.size && result == 0;) {
        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        pos = res->size;
        hash_time_list_resize(res, pos + 1);
        result = hash_time_set_tlv(res->data[pos], &_tlv_data);
    }

    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void hash_time_list_get_tlv(const struct hash_time_list_st *hash_time_list, struct string_st *res) {
    if (res == NULL) return;
    string_clear(res);
    if (hash_time_list == NULL) return;

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    for (size_t i = 0, size = hash_time_list->size; i < size; i++)  {
        hash_time_get_tlv(hash_time_list->data[i], &_tlv_data);
        string_concat(res, &_tlv_data);
    }
    tlv_set_string(res, TLV_HASH_TIME_LIST, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *hash_time_list_subscript(struct error_st *err, struct hash_time_list_st *list, const struct object_st *obj) {
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
            object_set_type(res, HASH_TIME_TYPE);
            hash_time_set(res->data, list->data[integer_get_ui(temp->data) % list->size]);
        }
        object_free(temp);
        return res;
    }
    res = object_new();
    if (res == NULL) {
        error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
        return NULL;
    }
    object_set_type(res, HASH_TIME_TYPE);
    hash_time_set(res->data, list->data[integer_get_ui(obj->data) % list->size]);
    return res;
}
