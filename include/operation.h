#ifndef OP_STRUCTS_H
#define OP_STRUCTS_H

#include "smart.h"

#include "operation/op_class.h"
#include "operation/op_function.h"
#include "operation/op_object.h"


#define TLV_OP_CLASS         0x9F10
#define TLV_OP_FUNCTION      0x9F11
#define TLV_OP_OBJECT        0x9F12


#define OP_CLASS_SIZE       sizeof(struct op_class)
#define OP_FUNCTION_SIZE    sizeof(struct op_function)
#define OP_OBJECT_SIZE      sizeof(struct op_object)


#define OP_CLASS_OP     {METHOD_NEW &op_class_new, METHOD_FREE &op_class_free, METHOD_SET &op_class_set, METHOD_SET &op_class_copy, METHOD_MARK &op_class_mark, METHOD_MARK &op_class_unmark, METHOD_CLEAR &op_class_clear, METHOD_CMP &op_class_cmp}
#define OP_FUNCTION_OP  {METHOD_NEW &op_function_new, METHOD_FREE &op_function_free, METHOD_SET &op_function_set, METHOD_SET &op_function_copy, METHOD_MARK &op_function_mark, METHOD_MARK &op_function_unmark, METHOD_CLEAR &op_function_clear, METHOD_CMP &op_function_cmp}
#define OP_OBJECT_OP    {METHOD_NEW &op_object_new, METHOD_FREE &op_object_free, METHOD_SET &op_object_set, METHOD_SET &op_object_copy, METHOD_MARK &op_object_mark, METHOD_MARK &op_object_unmark, METHOD_CLEAR &op_object_clear, METHOD_CMP &op_object_cmp}


extern struct object_type op_class_type;
extern struct object_type op_function_type;
extern struct object_type op_object_type;


#define OP_CLASS_TYPE       &op_class_type
#define OP_FUNCTION_TYPE    &op_function_type
#define OP_OBJECT_TYPE      &op_object_type

#endif //OP_STRUCTS_H
