#include "restore.h"

struct restore_tree_st *restore_tree_new() {
    struct restore_tree_st *res = malloc(sizeof(struct restore_tree_st));
    restore_list_data_init(&res->nodes);
    res->height = 0;

    string_data_init(&res->hash);

    res->block = NULL;
    res->stack = NULL;
    return res;
}
void restore_tree_free(struct restore_tree_st *res) {
    if (res == NULL) return;
    restore_list_data_free(&res->nodes);

    string_data_free(&res->hash);

    if (res->block != NULL) restore_block_free(res->block);
    if (res->stack != NULL) restore_stack_free(res->stack);
    free(res);
}

void restore_tree_set(struct restore_tree_st *res, const struct restore_tree_st *a) {
    if (res == NULL) return;
    if (a == NULL) return restore_tree_clear(res);
    restore_list_set(&res->nodes, &a->nodes);
    res->height = a->height;

    string_set(&res->hash, &a->hash);
}
void restore_tree_copy(struct restore_tree_st *res, const struct restore_tree_st *a) {
    if (res == NULL) return;
    if (a == NULL) return restore_tree_clear(res);
    restore_list_copy(&res->nodes, &a->nodes);
    res->height = a->height;

    string_copy(&res->hash, &a->hash);
}

void restore_tree_clear(struct restore_tree_st *res) {
    if (res == NULL) return;
    restore_list_clear(&res->nodes);
    res->height = 0;

    string_clear(&res->hash);

    if (res->block != NULL) restore_block_clear(res->block);
    if (res->stack != NULL) restore_stack_clear(res->stack);
}
int8_t restore_tree_cmp(const struct restore_tree_st *obj1, const struct restore_tree_st *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(&obj1->hash, &obj2->hash)) return CMP_NEQ;
    return CMP_EQ;
}

// Data Methods
void restore_tree_data_init(struct restore_tree_st *res) {
    if (res == NULL) return;
    restore_list_data_init(&res->nodes);
    res->height = 0;

    string_data_init(&res->hash);

    res->block = NULL;
    res->stack = NULL;
}
void restore_tree_data_free(struct restore_tree_st *res) {
    if (res == NULL) return;
    restore_list_data_free(&res->nodes);

    string_data_free(&res->hash);

    if (res->block != NULL) restore_block_free(res->block);
    if (res->stack != NULL) restore_stack_free(res->stack);
}

// TLV Methods
int8_t restore_tree_set_tlv(struct restore_tree_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    restore_tree_clear(res);
    int32_t tag = tlv_get_tag(tlv);
    if (tag < 0) return (int8_t) tag;
    if (tag != TLV_RESTORE_TREE) return ERR_TLV_TAG;

    struct string_st _tlv, _tlv_data;
    string_data_init(&_tlv_data);
    string_data_init(&_tlv);
    int8_t result;
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = size_set_tlv(&res->height, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = restore_list_set_tlv(&res->nodes, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = string_set_tlv(&res->hash, &_tlv_data))) goto end;
    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    return result;
}
void restore_tree_get_tlv(const struct restore_tree_st *tree, struct string_st *res) {
    if (res == NULL) return;
    if (tree == NULL) return string_clear(res);

    struct string_st _tlv_data;
    string_data_init(&_tlv_data);
    size_get_tlv(tree->height, res);

    restore_list_get_tlv(&tree->nodes, &_tlv_data);
    string_concat(res, &_tlv_data);

    string_get_tlv(&tree->hash, &_tlv_data);
    string_concat(res, &_tlv_data);

    tlv_set_string(res, TLV_RESTORE_TREE, res);
    string_data_free(&_tlv_data);
}