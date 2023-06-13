#include "operation.h"
struct object_sub op_object_sub = {METHOD_SUBSCRIPT &op_object_subscript, METHOD_ATTRIB &op_object_attrib};
//struct object_math_op op_object_math = {METHOD_MATH &op_object__mod, METHOD_MATH &op_object__and, METHOD_MATH &op_object__mul, METHOD_MATH &op_object__add, METHOD_MATH &op_object__sub, METHOD_MATH &op_object__div, METHOD_MATH &op_object__xor, METHOD_MATH &op_object__or, METHOD_MATH &op_object__ls, METHOD_MATH &op_object__rs, METHOD_CONVERT &op_object__neg};
//struct object_convert op_object_convert = {METHOD_CONVERT &op_object__bool, METHOD_CONVERT &op_object__int, METHOD_CONVERT &op_object__float, METHOD_CONVERT &op_object__str};
//struct object_type op_object_type = {OP_OBJECT_OP, NULL, &op_object_sub, &op_object_convert, &op_object_math};
struct object_tlv op_object_tlv = {METHOD_GET_TLV &op_object_get_tlv, METHOD_SET_TLV &op_object_set_tlv};
struct object_type op_object_type = {OP_OBJECT_OP, &op_object_tlv, &op_object_sub};

struct op_object *op_object_new() {
    struct op_object *res = skr_malloc(OP_OBJECT_SIZE);
    res->attr = NULL;

    res->closure = NULL;
    res->argument = 0;
    res->class_body = 0;

    return res;
}
void op_object_free(struct op_object *res) {
    if (res == NULL) return;
    if(res->closure != NULL) frame_free(res->closure);
    if(res->attr != NULL) frame_free(res->attr);

    skr_free(res);
}

void op_object_set(struct op_object *res, const struct op_object *a) {
    if (res == NULL) return;
    op_object_clear(res);
    if (a == NULL) return;
    if (a->attr != NULL) {
        res->attr = frame_new();
        frame_set(res->attr, a->attr);
    }

    if (a->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, a->closure);
    }
    res->argument = a->argument;
    res->class_body = a->class_body;
}
void op_object_copy(struct op_object *res, const struct op_object *a) {
    if (res == NULL) return;
    op_object_clear(res);
    if (a == NULL) return;
    if (a->attr != NULL) {
        res->attr = frame_new();
        frame_set(res->attr, a->attr);
    }

    if (a->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, a->closure);
    }
    res->argument = a->argument;
    res->class_body = a->class_body;
}

void op_object_mark(struct op_object *res) {
    if (res == NULL) return;
    frame_mark(res->attr);
    frame_mark(res->closure);
}
void op_object_unmark(struct op_object *res) {
    if (res == NULL) return;
    frame_unmark(res->attr);
    frame_unmark(res->closure);
}

void op_object_clear(struct op_object *res) {
    if (res == NULL) return;
    if(res->attr != NULL) frame_free(res->attr);
    if(res->closure != NULL) frame_free(res->closure);
    res->attr = NULL;
    res->closure = NULL;
    res->argument = 0;
    res->class_body = 0;
}
int op_object_cmp(const struct op_object *obj1, const struct op_object *obj2) {
    if (obj1 == NULL || obj2 == NULL || obj1->class_body != obj2->class_body) return CMP_NEQ;
    return CMP_EQ;
}

void op_object_define(struct op_object *res, struct op_class *class) {
//    map_copy(res->attr, class->attr);


    if (class->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, class->closure);
    }
    res->argument = class->argument;
    res->class_body = class->class_body;
}

// TLV Methods
int op_object_set_tlv(struct op_object *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    op_object_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    if (result != TLV_OP_OBJECT) return ERR_TLV_TAG;

    res->attr = frame_new();
    res->closure = frame_new();
    struct integer_st position;
    integer_data_init(&position);

    struct string_st _tlv = {NULL, 0, 0}, _tlv_data  = {NULL, 0, 0};
    if ((result = tlv_get_value(tlv, &_tlv))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = frame_set_tlv(res->attr, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = frame_set_tlv(res->closure, &_tlv_data))) goto end;

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&position, &_tlv_data))) goto end;
    res->argument = integer_get_si(&position);

    if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end;
    if ((result = integer_set_tlv(&position, &_tlv_data))) goto end;
    res->class_body = integer_get_si(&position);

    end:
    string_data_free(&_tlv);
    string_data_free(&_tlv_data);
    integer_data_free(&position);
    return result;
}
void op_object_get_tlv(const struct op_object *res, struct string_st *tlv) {
    if (tlv == NULL) return;
    if (res == NULL) return string_clear(tlv);

    struct integer_st position;
    integer_data_init(&position);
    struct string_st _tlv_data = {NULL, 0, 0};
    frame_get_tlv(res->attr, tlv);

    frame_get_tlv(res->closure, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    integer_set_ui(&position, res->argument);
    integer_get_tlv(&position, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    integer_set_ui(&position, res->class_body);
    integer_get_tlv(&position, &_tlv_data);
    string_concat(tlv, &_tlv_data);

    tlv_set_string(tlv, TLV_OP_OBJECT, tlv);
    string_data_free(&_tlv_data);
    integer_data_free(&position);
}


// Sub method
struct object_st *op_object_subscript(struct error_st *err, struct op_object *obj, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    struct frame_st *frame = obj->attr;
    if (obj2 == NULL) {
        error_set_msg(err, ErrorType_Math, "Can not make operation with object None");
        return NULL;
    }
    struct object_st *res = NULL;
    if (obj2->type != STRING_TYPE) {
        struct object_st *temp = object_new();
        object__int(temp, err, obj2);

        if (err == NULL || !err->present) {
            for (size_t i = 0, size = frame->attrib.size; i < size; i++) {
                if (string_cmp(&frame->attrib.variables[i]->name, temp->data) == 0)
                    res = object_copy_obj(frame->data.data[i]);
            }
            if (res == NULL) {
                string_set(&frame->attrib.variables[variable_list_add_new(&frame->attrib)]->name, temp->data);
                list_resize(&frame->data, frame->data.size + 1);
                frame->data.data[frame->data.size - 1] = object_new();
            }
        }
        object_free(temp);
        return res;
    }
    for (size_t i = 0, size = frame->attrib.size; i < size; i++) {
        if (string_cmp(&frame->attrib.variables[i]->name, obj2->data) == 0)
            res = object_copy_obj(frame->data.data[i]);
    }
    if (res == NULL) {
        string_set(&frame->attrib.variables[variable_list_add_new(&frame->attrib)]->name, obj2->data);
        list_resize(&frame->data, frame->data.size + 1);
        frame->data.data[frame->data.size - 1] = object_new();
    }
    return res;
}
struct object_st *op_object_attrib(struct error_st *err, const struct op_object *obj, const struct string_st *str) {
    struct frame_st *frame = obj->attr;
    struct object_st *res = NULL;
    for (size_t i = 0, size = frame->attrib.size; i < size; i++) {
        if (string_cmp(&frame->attrib.variables[i]->name, str) == 0)
            res = object_copy_obj(frame->data.data[i]);
    }
    if (res == NULL) {
        string_set(&frame->attrib.variables[variable_list_add_new(&frame->attrib)]->name, str);
        list_resize(&frame->data, frame->data.size + 1);
        frame->data.data[frame->data.size - 1] = object_new();
    }
    return res;
}
