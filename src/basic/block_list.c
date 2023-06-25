#include "basic.h"

struct object_sub block_list_sub = {METHOD_SUBSCRIPT &block_list_subscript};
struct object_tlv block_list_tlv = {METHOD_GET_TLV &block_list_get_tlv, METHOD_SET_TLV &block_list_set_tlv};
struct object_type block_list_type = {BLOCK_LIST_OP, &block_list_tlv, &block_list_sub};

// Standard operations
struct block_list_st *block_list_new() {
    struct block_list_st *res = malloc(sizeof(struct block_list_st));
    res->addresses = NULL;
    res->times = NULL;
    res->max_size = 0;
    res->size = 0;
    return res;
}
void block_list_free(struct block_list_st *res) {
    if (res == NULL) return;

    block_list_resize(res, 0);
    if (res->addresses != NULL) free(res->addresses);
    if (res->times != NULL) free(res->times);
    free(res);
}

void block_list_set(struct block_list_st *res, const struct block_list_st *a) {
    if (res == NULL) return;
    if (a == NULL) return block_list_clear(res);

    block_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) string_set(res->addresses[i], a->addresses[i]);
    for (size_t i = 0, size = a->size; i < size; i ++) integer_set(res->times[i], a->times[i]);
}
void block_list_copy(struct block_list_st *res, const struct block_list_st *a) {
    if (res == NULL) return;
    if (a == NULL) return block_list_clear(res);

    block_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i ++) string_set(res->addresses[i], a->addresses[i]);
    for (size_t i = 0, size = a->size; i < size; i ++) integer_set(res->times[i], a->times[i]);
}

void block_list_clear(struct block_list_st *res) {
    if (res == NULL) return;
    block_list_resize(res, 0);
}
int block_list_cmp(const struct block_list_st *obj1, const struct block_list_st *obj2) {
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    if (obj1->size > obj2->size) return CMP_GRET;
    if (obj1->size < obj2->size) return CMP_LESS;
    int res_cmp_sub;
    for (size_t i = 0, size = obj1->size; i < size; i++) {
        res_cmp_sub = string_cmp(obj1->addresses[i], obj2->addresses[i]);
        if (res_cmp_sub != CMP_EQ) return res_cmp_sub;
        res_cmp_sub = integer_cmp(obj1->times[i], obj2->times[i]);
        if (res_cmp_sub != CMP_EQ) return res_cmp_sub;
    }
    return CMP_EQ;
}

// Data Methods
void block_list_data_init(struct block_list_st *res) {
    if (res == NULL) return;
    res->addresses = NULL;
    res->times = NULL;
    res->max_size = 0;
    res->size = 0;
}
void block_list_data_free(struct block_list_st *res) {
    if (res == NULL) return;
    block_list_resize(res, 0);
    if (res->addresses != NULL) free(res->addresses);
    if (res->times != NULL) free(res->times);
}

// Cmp Methods
int block_list_is_null(const struct block_list_st *res) {
    return (res == NULL || res->size == 0);
}
void block_list_resize(struct block_list_st *res, size_t size) {
    if (res->addresses == NULL && size != 0) {
        res->max_size = size;
        res->addresses = malloc(sizeof(struct string_st *) * size);
        res->times = malloc(sizeof(struct string_st *) * size);
        for (size_t i = 0; i < size; i++) res->addresses[i] = NULL;
        for (size_t i = 0; i < size; i++) res->times[i] = NULL;
    } else if (res->max_size < size) {
        res->addresses = realloc(res->addresses, sizeof(struct string_st *) * size * 2);
        res->times = realloc(res->times, sizeof(struct string_st *) * size * 2);
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->addresses[i] = NULL;
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->times[i] = NULL;
        res->max_size = size * 2;
    }
    for (size_t i = size, l = res->size; i < l; i++) {
        if (res->addresses[i] != NULL) string_free(res->addresses[i]);
        if (res->times[i] != NULL) integer_free(res->times[i]);
        res->addresses[i] = NULL;
        res->times[i] = NULL;
    }
    for (size_t i = res->size, l = size; i < l; i++) {
        if (res->addresses[i] == NULL) res->addresses[i] = string_new();
        if (res->times[i] == NULL) res->times[i] = integer_new();
    }
    res->size = size;
}
void block_list_sort_merge(size_t st1, size_t fn1, size_t st2, size_t fn2, struct block_list_st *data, struct block_list_st *temp) {
    size_t st = st1;
    size_t pos = st1;
    while (st1 < fn1 || st2 < fn2) {
        if (st1 == fn1) {
            string_set(temp->addresses[pos], data->addresses[st2]);
            integer_set(temp->times[pos++], data->times[st2++]);
        } else if (st2 == fn2) {
            string_set(temp->addresses[pos], data->addresses[st1]);
            integer_set(temp->times[pos++], data->times[st1++]);
        } else {
            if (integer_cmp(data->times[st1], data->times[st2]) <= 0 ||
                string_cmp(data->addresses[st1], data->addresses[st2]) <= 0) {
                string_set(temp->addresses[pos], data->addresses[st1]);
                integer_set(temp->times[pos++], data->times[st1++]);
            } else {
                string_set(temp->addresses[pos], data->addresses[st2]);
                integer_set(temp->times[pos++], data->times[st2++]);
            }
        }
    }
    for (; st < fn2; st++) {
        string_set(data->addresses[st], temp->addresses[st]);
        integer_set(data->times[st], temp->times[st]);
    }
}
void block_list_sort_split(size_t st, size_t fn, struct block_list_st *data, struct block_list_st *temp) {
    if (st + 1 >= fn) return;
    size_t mid = (st + fn) / 2;
    block_list_sort_split(st, mid, data, temp);
    block_list_sort_split(mid, fn, data, temp);
    block_list_sort_merge(st, mid, mid, fn, data, temp);
}
void block_list_sort(struct block_list_st *res) {
    if(res == NULL) return;

    struct block_list_st temp;
    block_list_data_init(&temp);

    block_list_resize(&temp, res->size);
    block_list_sort_split(0, res->size, res, &temp);

    block_list_data_free(&temp);
}

// TLV Methods
int block_list_set_tlv(struct block_list_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    block_list_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_BLOCK_LIST) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    result = tlv_get_value(tlv, &_tlv);

    for (size_t pos; _tlv.size && result == 0;) {
        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        pos = res->size;
        block_list_resize(res, pos + 1);
        if ((result = string_set_tlv(res->addresses[pos], &_tlv_data))) break;

        if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) break;
        result = integer_set_tlv(res->times[pos], &_tlv_data);
    }

    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void block_list_get_tlv(const struct block_list_st *block_list, struct string_st *res) {
    if (res == NULL) return;
    string_clear(res);
    if (block_list == NULL) return;

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    for (size_t i = 0, size = block_list->size; i < size; i++)  {
        string_get_tlv(block_list->addresses[i], &_tlv_data);
        string_concat(res, &_tlv_data);

        integer_get_tlv(block_list->times[i], &_tlv_data);
        string_concat(res, &_tlv_data);
    }
    tlv_set_string(res, TLV_BLOCK_LIST, res);
    string_data_free(&_tlv_data);
}

// Attrib Methods
struct object_st *block_list_subscript(struct error_st *err, struct block_list_st *list, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj == NULL) {
        error_set_msg(err, ErrorType_Math, "Can not make operation with object None");
        return NULL;
    }
    struct object_st *res = NULL;
    if (obj->type != INTEGER_TYPE) {
        struct object_st *temp = object_new();
        object__int(temp, err, obj);

        if (err == NULL || !err->present) {
            res = object_new();
            object_set_type(res, STRING_TYPE);
            string_set(res->data, list->addresses[integer_get_ui(temp->data) % list->size]);
        }
        object_free(temp);
        return res;
    }
    res = object_new();
    object_set_type(res, STRING_TYPE);
    string_set(res->data, list->addresses[integer_get_ui(obj->data) % list->size]);
    return res;
}
