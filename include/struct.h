#ifndef STRUCT_H
#define STRUCT_H

#include "memory.h"

#include "struct/object.h"
#include "struct/string.h"
#include "struct/tlv.h"
#include "struct/integer.h"
#include "struct/float.h"
#include "struct/linked_list.h"
#include "struct/list.h"
#include "struct/error.h"

#define USE_GMP

#define ERR_SUCCESS         0
#define ERR_TLV_TAG         (-0x01)
#define ERR_TLV_VALUE       (-0x02)

#define ERR_DATA_NULL       (-0x11)
#define ERR_DATA_CHECK      (-0x12)

#define ERR_FILE_OPEN       (-0x21)
#define ERR_FILE_READ       (-0x22)
#define ERR_FILE_WRITE      (-0x23)
#define ERR_FILE_REMOVE     (-0x24)

#define CMP_LESS            (-1)
#define CMP_GRET            1
#define CMP_EQ              0
#define CMP_NEQ             2

#define CHECK_TRUE          1
#define CHECK_FALSE         0


#define TLV_INTEGER         0xC1
#define TLV_LIST            0xF0
#define TLV_LINKED_LIST     0xF1
#define TLV_STRING          0xCC
#define TLV_POINTER_DEF     0xA0
#define TLV_POINTER         0xA1
#define TLV_POINTER_END     0xA2


#define INTEGER_OP      (struct object_op) {METHOD_NEW &integer_new, METHOD_FREE &integer_free, METHOD_SET &integer_set, METHOD_SET &integer_copy, NULL, NULL, METHOD_CLEAR &integer_clear, METHOD_CMP &integer_cmp}
#define FLOAT_OP        (struct object_op) {METHOD_NEW &float_new, METHOD_FREE &float_free, METHOD_SET &float_set, METHOD_SET &float_copy, NULL, NULL, METHOD_CLEAR &float_clear, METHOD_CMP &float_cmp}
#define LINKED_LIST_OP  (struct object_op) {METHOD_NEW &linked_list_new, METHOD_FREE &linked_list_free, METHOD_SET &linked_list_set, METHOD_SET &linked_list_copy, METHOD_MARK &linked_list_mark, METHOD_MARK &linked_list_unmark, METHOD_CLEAR &linked_list_clear, METHOD_CMP &linked_list_cmp}
#define LIST_OP         (struct object_op) {METHOD_NEW &list_new, METHOD_FREE &list_free, METHOD_SET &list_set, METHOD_SET &list_copy, METHOD_MARK &list_mark, METHOD_MARK &list_unmark, METHOD_CLEAR &list_clear, METHOD_CMP &list_cmp}
#define OBJECT_OP       (struct object_op) {METHOD_NEW &object_new, METHOD_FREE &object_free, METHOD_SET &object_set, METHOD_SET &object_copy, METHOD_MARK &object_mark, METHOD_MARK &object_unmark, METHOD_CLEAR &object_clear, METHOD_CMP &object_cmp}
#define STRING_OP       (struct object_op) {METHOD_NEW &string_new, METHOD_FREE &string_free, METHOD_SET &string_set, METHOD_SET &string_copy, NULL, NULL, METHOD_CLEAR &string_clear, METHOD_CMP &string_cmp}
#define ERROR_OP        (struct object_op) {METHOD_NEW &error_new, METHOD_FREE &error_free, METHOD_SET &error_set, METHOD_SET &error_copy, NULL, NULL, METHOD_CLEAR &error_clear, METHOD_CMP &error_cmp}
#define TLV_OP          STRING_OP


extern struct object_type integer_type;
extern struct object_type float_type;
extern struct object_type linked_list_type;
extern struct object_type list_type;
extern struct object_type object_type;
extern struct object_type string_type;
extern struct object_type error_type;
extern struct object_type tlv_type;


#define NONE_TYPE           NULL
#define INTEGER_TYPE        &integer_type
#define FLOAT_TYPE          &float_type
#define LINKED_LIST_TYPE    &linked_list_type
#define LIST_TYPE           &list_type
#define OBJECT_TYPE         &object_type
#define STRING_TYPE         &string_type
#define ERROR_TYPE          &error_type
#define TLV_TYPE            &tlv_type



#endif //STRUCT_H
