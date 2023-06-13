#include "struct.h"
#include "basic.h"
#include "operation.h"

struct object_sub object_sub = {METHOD_SUBSCRIPT &object_subscript, METHOD_ATTRIB &object_attrib};
struct object_tlv object_tlv = {METHOD_GET_TLV &object_get_tlv, METHOD_SET_TLV &object_set_tlv};
struct object_math_op object_math = {METHOD_MATH &object__mod, METHOD_MATH &object__and, METHOD_MATH &object__mul, METHOD_MATH &object__add, METHOD_MATH &object__sub, METHOD_MATH &object__div, METHOD_MATH &object__xor, METHOD_MATH &object__or, METHOD_MATH &object__ls, METHOD_MATH &object__rs, METHOD_CONVERT &object__neg};
struct object_convert object_convert = {METHOD_CONVERT &object__bool, METHOD_CONVERT &object__int, METHOD_CONVERT &object__float, METHOD_CONVERT &object__str};
struct object_type object_type = {OBJECT_OP, &object_tlv, &object_sub, &object_convert, &object_math};

int marked = 0;
// Standard operations
struct object_st *object_new() {
    struct object_st *res = skr_malloc(sizeof(struct object_st));
    res->type = NULL;
    res->data = NULL;
    res->counter = 1;
    res->flag = 0;
    return res;
}
void object_free(struct object_st *res) {
    if (res == NULL) return;
    int marked_ = !marked;
    if (res->flag == 0 || marked_) {
        marked = marked_;
        object_unmark(res);
        object_mark(res);
        if (res->flag != res->counter) {
            res->counter--;
            return object_unmark(res);
        }
    }
    if (res->flag-- == res->counter && res->data != NULL) {
        if (res->type != NULL && res->type->self._free != NULL)
            res->type->self._free(res->data);
        res->data = NULL;
    }
    res->counter--;
    if (res->counter == 0) skr_free(res);
    if (marked_) marked = !marked;
}

void object_set(struct object_st *res, const struct object_st *a) {
    if (res == NULL || a == NULL) return;
    object_set_type(res, a->type);
    if (res->type != NULL && res->type->self._set != NULL) res->type->self._set(res->data, (a)->data);
}
void object_copy(struct object_st *res, const struct object_st *a) {
    if (res == NULL || a == NULL) return;
    object_set_type(res, a->type);
    if (res->type != NULL && res->type->self._copy != NULL) res->type->self._copy(res->data, (a)->data);
}

void object_mark(struct object_st *res) {
    if (res == NULL || res->flag++) return;
    if (res->type != NULL && res->type->self._mark != NULL) return res->type->self._mark(res->data);
}
void object_unmark(struct object_st *res) {
    if (res == NULL || !res->flag) return;
    res->flag = 0;
    if (res->type != NULL && res->type->self._unmark != NULL) res->type->self._unmark(res->data);
}

void object_clear(struct object_st *res) {
    if (res == NULL) return;
    object_set_type(res, NONE_TYPE);
}
int object_cmp(const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;

    if (obj1 == obj2) return CMP_EQ;
    if (obj1 == NULL || obj2 == NULL) return CMP_NEQ;
    if (obj1->type != obj2->type) return CMP_NEQ;
    if (obj1->type != NULL && obj1->type->self._cmp != NULL) return obj1->type->self._cmp(obj1->data, obj2->data);
    return CMP_EQ;
}

// Class Methods
struct object_st *object_copy_obj(struct object_st *res) {
    if (res == NULL) return NULL;
    res->counter++;
    return res;
}
void object_set_type(struct object_st *res, struct object_type *type) {
    if (res == NULL) return;
    if (res->type == type) return;
    if (res->type != NULL) {
        if (res->data != NULL) {
            if (res->type != NULL && res->type->self._free != NULL)
                res->type->self._free(res->data);
            res->data = NULL;
        }
    }
    res->type = type;
    if (res->type != NULL && res->type->self._new != NULL) res->data = res->type->self._new();
}
void object_set_pointer(struct object_st *res, struct object_st *obj) {
    if (res == NULL || obj == NULL) return;
    object_set_type(res, NONE_TYPE);
    res->data = object_copy_obj(obj);
    res->type = OBJECT_TYPE;
}

// TLV method
struct list_st *tlv_objects = NULL;
int find_in_process_tlv(const struct object_st *res) {
    if (list_is_null(tlv_objects))
        for (size_t i = 0, size = tlv_objects->size; i < size; i++)
            if (res == tlv_objects->data[i]) return (int) i;
    return -1;
}

int object_set_tlv(struct object_st *res, const struct string_st *tlv) {
    if (res == NULL) return ERR_DATA_NULL;
    object_clear(res);
    int result = tlv_get_tag(tlv);
    if (result < 0) return result;
    switch (result) {
        // struct
        case TLV_INTEGER:
            object_set_type(res, INTEGER_TYPE);
            return integer_set_tlv(res->data, tlv);
        case TLV_LIST:
            object_set_type(res, LIST_TYPE);
            return list_set_tlv(res->data, tlv);
        case TLV_STRING:
            object_set_type(res, STRING_TYPE);
            return string_set_tlv(res->data, tlv);
        // basic
        case TLV_ACCOUNT:
            object_set_type(res, ACCOUNT_TYPE);
            return account_set_tlv(res->data, tlv);
        case TLV_ACCOUNT_CONN:
            object_set_type(res, ACCOUNT_CONN_TYPE);
            return account_connections_set_tlv(res->data, tlv);
        case TLV_ACTIVE_ACC:
            object_set_type(res, ACTIVE_ACC_TYPE);
            return activated_accounts_set_tlv(res->data, tlv);
        case TLV_BLOCK:
            object_set_type(res, BLOCK_TYPE);
            return block_set_tlv(res->data, tlv);
        case TLV_BLOCK_HISTORY:
            object_set_type(res, BLOCK_HISTORY_TYPE);
            return block_history_set_tlv(res->data, tlv);
        case TLV_CURRENCY:
            object_set_type(res, CURRENCY_TYPE);
            return currency_set_tlv(res->data, tlv);
        case TLV_GENERATION:
            object_set_type(res, GENERATION_TYPE);
            return generation_set_tlv(res->data, tlv);
        case TLV_TRANSACTION:
            object_set_type(res, TRANSACTION_TYPE);
            return transaction_set_tlv(res->data, tlv);
        case TLV_SMARTCONTRACT:
            object_set_type(res, SMARTCONTRACT_TYPE);
            return smartcontract_set_tlv(res->data, tlv);
        case TLV_WALLET:
            object_set_type(res, WALLET_TYPE);
            return wallet_set_tlv(res->data, tlv);
        case TLV_WALLET_DATA:
            object_set_type(res, WALLET_DATA_TYPE);
            return wallet_data_set_tlv(res->data, tlv);
        case TLV_WALLET_SMART:
            object_set_type(res, WALLET_SMART_TYPE);
            return wallet_smart_set_tlv(res->data, tlv);
        // basic
        case TLV_OP_CLASS:
            object_set_type(res, OP_CLASS_TYPE);
            return op_class_set_tlv(res->data, tlv);
        case TLV_OP_FUNCTION:
            object_set_type(res, OP_FUNCTION_TYPE);
            return op_function_set_tlv(res->data, tlv);
        case TLV_OP_OBJECT:
            object_set_type(res, OP_OBJECT_TYPE);
            return op_object_set_tlv(res->data, tlv);

        // pointer
        case TLV_POINTER_DEF:{
            struct string_st _tlv = {NULL, 0, 0}, _tlv_data  = {NULL, 0, 0};
            struct integer_st position;
            integer_data_init(&position);
            if ((result = tlv_get_value(tlv, &_tlv))) goto end_def;

            if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end_def;
            if ((result = integer_set_tlv(&position, &_tlv_data))) goto end_def;

            size_t pos = integer_get_si(&position);

            if (tlv_objects == NULL)
                tlv_objects = list_new();

            if (tlv_objects->size <= pos)
                list_resize(tlv_objects, pos);

            tlv_objects->data[pos] = res;

            if ((result = tlv_get_next_tlv(&_tlv, &_tlv_data))) goto end_def;
            if ((result = object_set_tlv(res, &_tlv_data))) goto end_def;

            end_def:
            integer_data_free(&position);
            string_data_free(&_tlv);
            string_data_free(&_tlv_data);
            return result;
        }
        case TLV_POINTER: {
            struct integer_st position;
            integer_data_init(&position);

            if ((result = integer_set_tlv_(&position, tlv))) goto end_ptr;
            size_t pos = integer_get_si(&position);

            if (tlv_objects->data[pos] != NULL) object_set_pointer(res, tlv_objects->data[pos]);
            else result = ERR_TLV_VALUE;

            end_ptr:
            integer_data_free(&position);
            return result;
        }
        case TLV_POINTER_END: {
            struct integer_st position;
            integer_data_init(&position);

            if ((result = integer_set_tlv_(&position, tlv))) goto end_end;
            size_t pos = integer_get_si(&position);

            if (tlv_objects->data[pos] != NULL) object_set_pointer(res, tlv_objects->data[pos]);
            else result = ERR_TLV_VALUE;

            tlv_objects->data[pos] = NULL;

            end_end:
            integer_data_free(&position);
            return result;
        }

        default:
            return ERR_TLV_TAG;
    }
}
void object_get_tlv(struct object_st *res, struct string_st *tlv) {
    while (res != NULL && res->type == OBJECT_TYPE) res = res->data;
    if (res == NULL || res->type == NULL) return;
    string_clear(tlv);

    int marked_ = !marked;
    if (res->flag == 0 || marked_) {
        marked = marked_;
        object_unmark(res);
        object_mark(res);
    }
    if (res->flag > 0) {
        if (res->flag-- != 1) {
            res->flag = -res->flag;
            if (tlv_objects == NULL)
                tlv_objects = list_new();

            size_t pos = tlv_objects->size;
            for (size_t i = 0; i < pos; i++)
                if (res == tlv_objects->data[i] || tlv_objects->data[i] == NULL) pos = i;
            if (pos == tlv_objects->size) list_resize(tlv_objects, tlv_objects->size + 1);
            tlv_objects->data[pos] = res;

            struct string_st _tlv_data = {NULL, 0, 0};
            struct integer_st position;
            integer_data_init(&position);

            integer_set_ui(&position, pos);
            integer_get_tlv(&position, tlv);

            if (res->type != NULL && res->type->tlv != NULL && res->type->tlv->_get_tlv != NULL)
                res->type->tlv->_get_tlv(res->data, &_tlv_data);
            string_concat(tlv, &_tlv_data);


            tlv_set_string(tlv, TLV_POINTER_DEF, tlv);

            integer_data_free(&position);
            string_data_free(&_tlv_data);
        } else {
            if (res->type != NULL && res->type->tlv != NULL && res->type->tlv->_get_tlv != NULL)
                return res->type->tlv->_get_tlv(res->data, tlv);
        }
    } else {
        size_t pos = tlv_objects->size;
        for (size_t i = 0; i < pos; i++)
            if (res == tlv_objects->data[i]) pos = i;

        struct integer_st position;
        integer_data_init(&position);

        integer_set_ui(&position, pos);
        if (++res->flag != 0) integer_get_tlv_(&position, tlv, TLV_POINTER);
        else {
            integer_get_tlv_(&position, tlv, TLV_POINTER_END);
            tlv_objects->data[pos] = NULL;
        }

        integer_data_free(&position);
    }
    if (marked_) marked = !marked;
}
int object_set_tlv_self(struct object_st *res, struct object_type *type) {
    if (res->type == type) return ERR_SUCCESS;
    if (res->type != TLV_TYPE) {
        object_set_type(res, type);
        return ERR_SUCCESS;
    }
    int result = ERR_SUCCESS;
    struct string_st _tlv_data = {NULL, 0, 0};
    string_set(&_tlv_data, res->data);

    object_set_type(res, type);
    if (res->type != NULL && res->type->tlv != NULL && res->type->tlv->_set_tlv != NULL)
        result = res->type->tlv->_set_tlv(res->data, &_tlv_data);
    string_data_free(&_tlv_data);
    return result;
}

// Sub method
struct object_st *object_subscript(struct error_st *err, struct object_st *obj, const struct object_st *obj2) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj != NULL && obj->type != NULL && obj->type->sub != NULL && obj->type->sub->_subscript != NULL) {
        return obj->type->sub->_subscript(err, obj->data, obj2);
    }
    error_set_msg(err, ErrorType_SubScript, "Object does not have __subscript__ operation");
    return NULL;
}
struct object_st *object_attrib(struct error_st *err, const struct object_st *obj, const struct string_st *str) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj != NULL && obj->type != NULL && obj->type->sub != NULL && obj->type->sub->_attrib != NULL) {
        return obj->type->sub->_attrib(err, obj->data, str);
    }
    error_set_msg(err, ErrorType_SubScript, "Object does not have __attrib__ operation");
    return NULL;
}

// Convert Methods
void object__bool(struct object_st *res, struct error_st *err, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj == NULL || obj->type == NULL) {
        object_set_type(res, INTEGER_TYPE);
        integer_set_ui(res->data, 0);
        return;
    }
    if (obj->type->convert != NULL && obj->type->convert->_bool != NULL) {
        obj->type->convert->_bool(res, err, obj->data);
    }else{
        object_set_type(res, INTEGER_TYPE);
        integer_set_ui(res->data, 1);
        return;
    }
}
void object__int(struct object_st *res, struct error_st *err, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj != NULL && obj->type != NULL && obj->type->convert != NULL && obj->type->convert->_int != NULL) {
        return obj->type->convert->_int(res, err, obj->data);
    }
    error_set_msg(err, ErrorType_Convert, "Object does not have __int__ operation");
}
void object__float(struct object_st *res, struct error_st *err, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj != NULL && obj->type != NULL && obj->type->convert != NULL && obj->type->convert->_float != NULL) {
        return obj->type->convert->_float(res, err, obj->data);
    }
    error_set_msg(err, ErrorType_Convert, "Object does not have __float__ operation");
}
void object__str(struct object_st *res, struct error_st *err, struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj->flag) {
        object_set_type(res, STRING_TYPE);
        return string_set_str(res->data, "...", 3);
    }
    obj->flag = 1;
    if (obj != NULL && obj->type != NULL && obj->type->convert != NULL && obj->type->convert->_str != NULL) {
        obj->type->convert->_str(res, err, obj->data);
        obj->flag = 0;
        return;
    }
    error_set_msg(err, ErrorType_Convert, "Object does not have __str__ operation");
    obj->flag = 0;
}

// Math Methods
void object__mod(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_mod != NULL) {
        return obj1->type->math->_mod(res, err, obj1->data, obj2);
    }
    error_set_msg(err, ErrorType_Math, "Object does not have __mod__ operation");
}
void object__and(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_and != NULL) {
        return obj1->type->math->_and(res, err, obj1->data, obj2);
    }
    error_set_msg(err, ErrorType_Math, "Object does not have __and__ operation");
}
void object__mul(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_mul != NULL) {
        return obj1->type->math->_mul(res, err, obj1->data, obj2);
    }
    error_set_msg(err, ErrorType_Math, "Object does not have __mul__ operation");
}
void object__add(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_add != NULL) {
        return obj1->type->math->_add(res, err, obj1->data, obj2);
    }
    error_set_msg(err, ErrorType_Math, "Object does not have __add__ operation");
}
void object__sub(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_sub != NULL) {
        return obj1->type->math->_sub(res, err, obj1->data, obj2);
    }
    error_set_msg(err, ErrorType_Math, "Object does not have __sub__ operation");
}
void object__div(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_div != NULL) {
        return obj1->type->math->_div(res, err, obj1->data, obj2);
    }
    error_set_msg(err, ErrorType_Math, "Object does not have __div__ operation");
}
void object__xor(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_xor != NULL) {
        return obj1->type->math->_xor(res, err, obj1->data, obj2);
    }
    error_set_msg(err, ErrorType_Math, "Object does not have __xor__ operation");
}
void object__or(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_or != NULL) {
        return obj1->type->math->_or(res, err, obj1->data, obj2);
    }
    error_set_msg(err, ErrorType_Math, "Object does not have __or__ operation");
}
void object__ls(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_ls != NULL) {
        return obj1->type->math->_ls(res, err, obj1->data, obj2);
    }
    error_set_msg(err, ErrorType_Math, "Object does not have __ls__ operation");
}
void object__rs(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_rs != NULL) {
        return obj1->type->math->_rs(res, err, obj1->data, obj2);
    }
    error_set_msg(err, ErrorType_Math, "Object does not have __rs__ operation");
}
void object__neg(struct object_st *res, struct error_st *err, const struct object_st *obj1) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_ne != NULL) {
        return obj1->type->math->_ne(res, err, obj1->data);
    }
    error_set_msg(err, ErrorType_Math, "Object does not have __neg__ operation");
}