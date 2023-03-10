#ifndef STRUCT_H
#define STRUCT_H

#include "memory.h"

#include "struct/object.h"
#include "struct/string.h"
#include "struct/tlv.h"
#include "struct/sub/sub_integer.h"
#include "struct/integer.h"
#include "struct/float.h"
#include "struct/list.h"
#include "struct/stack.h"
#include "struct/map.h"

#define USE_GMP

#define SUB_INTEGER_TLV    0xC0
#define INTEGER_TLV        0xC1
#define LIST_TLV           0xF0
#define STRING_TLV         0xCC


#define SUB_INTEGER_SIZE    sizeof(struct sub_integer)
#define INTEGER_SIZE        sizeof(struct integer_st)
#define FLOAT_SIZE          sizeof(struct float_st)
#define LIST_SIZE           sizeof(struct list_st)
#define STACK_SIZE          sizeof(struct stack_st)
#define POINTER_SIZE        sizeof(struct object_st *)
#define OBJECT_SIZE         sizeof(struct object_st)
#define STRING_SIZE         sizeof(struct string_st)
#define MAP_SIZE            sizeof(struct map_st)


#define SUB_INTEGER_OP  (struct object_op) {METHOD_NEW &sub_integer_new, METHOD_SET &sub_integer_set, METHOD_CLEAR &sub_integer_clear, METHOD_FREE &sub_integer_free, METHOD_CMP &sub_integer_cmp}
#define INTEGER_OP      (struct object_op) {METHOD_NEW &integer_new, METHOD_SET &integer_set, METHOD_CLEAR &integer_clear, METHOD_FREE &integer_free, METHOD_CMP &integer_cmp}
#define FLOAT_OP        (struct object_op) {METHOD_NEW &float_new, METHOD_SET &float_set, METHOD_CLEAR &float_clear, METHOD_FREE &float_free, METHOD_CMP &float_cmp}
#define LIST_OP         (struct object_op) {METHOD_NEW &list_new, METHOD_SET &list_set, METHOD_CLEAR &list_clear, METHOD_FREE &list_free, METHOD_CMP &list_cmp}
#define STACK_OP        (struct object_op) {METHOD_NEW &stack_new, METHOD_SET &stack_set, METHOD_CLEAR &stack_clear, METHOD_FREE &stack_free, METHOD_CMP &stack_cmp}
#define OBJECT_OP       (struct object_op) {METHOD_NEW &object_new, METHOD_SET &object_set, METHOD_CLEAR &object_clear, METHOD_FREE &object_free, METHOD_CMP &object_cmp}
#define STRING_OP       (struct object_op) {METHOD_NEW &string_new, METHOD_SET &string_set, METHOD_CLEAR &string_clear, METHOD_FREE &string_free, METHOD_CMP &string_cmp}
#define MAP_OP          (struct object_op) {METHOD_NEW &map_new, METHOD_SET &map_set, METHOD_CLEAR &map_clear, METHOD_FREE &map_free, METHOD_CMP &map_cmp}
#define TLV_OP          STRING_OP


extern struct object_type sub_integer_type;
extern struct object_type integer_type;
extern struct object_type float_type;
extern struct object_type list_type;
extern struct object_type object_type;
extern struct object_type stack_type;
extern struct object_type string_type;
extern struct object_type map_type;
extern struct object_type tlv_type;


#define NONE_TYPE           NULL
#define SUB_INTEGER_TYPE    &sub_integer_type
#define INTEGER_TYPE        &integer_type
#define FLOAT_TYPE          &float_type
#define LIST_TYPE           &list_type
#define OBJECT_TYPE         &object_type
#define STACK_TYPE          &stack_type
#define STRING_TYPE         &string_type
#define MAP_TYPE            &map_type
#define TLV_TYPE            &tlv_type



#endif //STRUCT_H
