#include "smart.h"


void node_list_set(struct node_list_st *res, const struct node_list_st *a) {
    if (res == NULL || res->type) return;
    node_list_resize(res, a->size);
    for (size_t i = 0, size = a->size; i < size; i++) res->nodes[i] = a->nodes[i];
}
void node_list_clear(struct node_list_st *res) {
    if (res == NULL) return;
    node_list_resize(res, 0);
}

void node_list_data_init(struct node_list_st *res) {
    if (res == NULL) return;
    res->nodes = NULL;
    res->max_size = 0;
    res->size = 0;
    res->type = 0;
}
void node_list_data_free(struct node_list_st *res) {
    if (res == NULL) return;
    node_list_resize(res, 0);
    if (res->nodes != NULL) free(res->nodes);
}

void node_list_resize(struct node_list_st *res, size_t size) {
    if (res->nodes == NULL && size != 0) {
        res->max_size = size;
        res->nodes = malloc(sizeof(struct node_st *) * size);
        for (size_t i = 0; i < size; i++) res->nodes[i] = NULL;
    } else if (res->max_size < size) {
        res->nodes = realloc(res->nodes, sizeof(struct node_st *) * size * 2);
        for (size_t i = res->max_size, l = size * 2; i < l; i++) res->nodes[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->type && res->size > size) {
        for (size_t i = size, l = res->size; i < l; i++) {
            if (res->nodes[i] != NULL) node_free(res->nodes[i]);
            res->nodes[i] = NULL;
        }
    }
    res->size = size;
}
void node_list_append(struct node_list_st *res, struct node_st *data) {
    if (res == NULL) return;
    node_list_resize(res, res->size + 1);
    res->nodes[res->size - 1] = data;
}
void node_list_add_new(struct node_list_st *res) {
    if (res == NULL || !res->type) return;
    node_list_resize(res, res->size + 1);
    res->nodes[res->size - 1] = node_new();
}
struct node_st *node_list_last(struct node_list_st *res) {
    if (res == NULL || res->nodes == NULL || res->size == 0) return NULL;
    return res->nodes[res->size - 1];
}