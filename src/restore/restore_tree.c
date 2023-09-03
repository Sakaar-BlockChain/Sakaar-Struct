#include "restore.h"

struct restore_tree_st *restore_tree_new() {
    struct restore_tree_st *res = malloc(sizeof(struct restore_tree_st));
    restore_list_data_init(&res->nodes);
    res->height = 0;

    integer_data_init(&res->block_id);
    string_data_init(&res->hash);
    return res;
}
void restore_tree_free(struct restore_tree_st *res) {
    if (res == NULL) return;
    restore_list_data_free(&res->nodes);

    integer_data_free(&res->block_id);
    string_data_free(&res->hash);
    free(res);
}

void restore_tree_set(struct restore_tree_st *res, const struct restore_tree_st *a) {
    if (res == NULL) return;
    if (a == NULL) return restore_tree_clear(res);
    restore_list_set(&res->nodes, &a->nodes);
    res->height = a->height;

    integer_set(&res->block_id, &a->block_id);
    string_set(&res->hash, &a->hash);
}
void restore_tree_copy(struct restore_tree_st *res, const struct restore_tree_st *a) {
    if (res == NULL) return;
    if (a == NULL) return restore_tree_clear(res);
    restore_list_copy(&res->nodes, &a->nodes);
    res->height = a->height;

    integer_copy(&res->block_id, &a->block_id);
    string_copy(&res->hash, &a->hash);
}

void restore_tree_clear(struct restore_tree_st *res) {
    if (res == NULL) return;
    restore_list_clear(&res->nodes);
    res->height = 0;

    integer_clear(&res->block_id);
    string_clear(&res->hash);
}
int8_t restore_tree_cmp(const struct restore_tree_st *obj1, const struct restore_tree_st *obj2) {
    if (obj1 == NULL || obj2 == NULL || string_cmp(&obj1->hash, &obj2->hash) || integer_cmp(&obj1->block_id, &obj2->block_id)) return CMP_NEQ;
    return CMP_EQ;
}

// Data Methods
void restore_tree_data_init(struct restore_tree_st *res) {
    if (res == NULL) return;
    restore_list_data_init(&res->nodes);
    res->height = 0;

    integer_data_init(&res->block_id);
    string_data_init(&res->hash);
}
void restore_tree_data_free(struct restore_tree_st *res) {
    if (res == NULL) return;
    restore_list_data_free(&res->nodes);

    integer_data_free(&res->block_id);
    string_data_free(&res->hash);
}