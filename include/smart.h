#ifndef SC_STRUCTS_H
#define SC_STRUCTS_H

#include "struct.h"

#include "smart/bytecode.h"
#include "smart/closure.h"
#include "smart/node.h"
#include "smart/token.h"
#include "smart/variable.h"

#include "smart/bytecode_list.h"
#include "smart/closure_list.h"
#include "smart/node_list.h"
#include "smart/token_list.h"
#include "smart/variable_list.h"
#include "smart/variable_list_list.h"


#include "smart/parser.h"
#include "smart/frame.h"


#define TLV_BYTECODE            0x7F01
#define TLV_BYTECODE_LIST       0x7F11
#define TLV_CLOSURE             0x7F02
#define TLV_CLOSURE_LIST        0x7F12
#define TLV_FRAME               0x7F03
#define TLV_VARIABLE            0x7F04
#define TLV_VARIABLE_LIST       0x7F14
#define TLV_VARIABLE_LIST_LIST  0x7F24


#endif //SC_STRUCTS_H
