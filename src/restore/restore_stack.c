#include "restore.h"

struct restore_stack_st *restore_stack_new() {
    struct restore_stack_st *res = malloc(sizeof(struct restore_stack_st));
    res->front = NULL;
    res->size = 0;
    return res;
}
void restore_stack_free(struct restore_stack_st *res) {
    if (res == NULL) return;

    struct restore_stack_elm_st *elm = res->front;
    struct restore_stack_elm_st *next = NULL;
    while (elm != NULL) {
        next = elm->next;
        restore_stack_elm_free(elm);
        elm = next;
    }
    free(res);
}

void restore_stack_clear(struct restore_stack_st *res) {
    if (res == NULL) return;
    struct restore_stack_elm_st *elm = res->front;
    struct restore_stack_elm_st *next = NULL;
    while (elm != NULL) {
        next = elm->next;
        restore_stack_elm_free(elm);
        elm = next;
    }

    res->front = NULL;
    res->size = 0;
}

// Data Methods
void restore_stack_data_init(struct restore_stack_st *res) {
    if (res == NULL) return;
    res->front = NULL;
    res->size = 0;
}
void restore_stack_data_free(struct restore_stack_st *res) {
    if (res == NULL) return;

    struct restore_stack_elm_st *elm = res->front;
    struct restore_stack_elm_st *next = NULL;
    while (elm != NULL) {
        next = elm->next;
        restore_stack_elm_free(elm);
        elm = next;
    }
}

void restore_stack_pop_front(struct restore_stack_st *res) {
    if (res == NULL || res->size == 0) return;

    struct restore_stack_elm_st *elm = res->front;
    res->size --;
    if (res->size == 0) {
        res->front = NULL;
    } else {
        res->front = elm->next;
        elm->next = NULL;
    }

    restore_stack_elm_free(elm);
}
void restore_stack_new_front(struct restore_stack_st *res) {
    if (res == NULL) return;

    struct restore_stack_elm_st *elm = restore_stack_elm_new();
    res->size++;
    if (res->size == 1) {
        res->front = elm;
    } else {
        elm->next = res->front;
        res->front = elm;
    }
}