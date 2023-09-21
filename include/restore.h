#ifndef RESTORE_H
#define RESTORE_H

#include "restore/restore_block.h"
#include "restore/restore_list.h"
#include "restore/restore_list_elm.h"
#include "restore/restore_queue.h"
#include "restore/restore_queue_elm.h"
#include "restore/restore_stack.h"
#include "restore/restore_stack_elm.h"
#include "restore/restore_tree.h"

#define TLV_RESTORE_BLOCK       0x1F61
#define TLV_RESTORE_LIST        0x1F62
#define TLV_RESTORE_LIST_ELM    0x1F63
#define TLV_RESTORE_QUEUE       0x1F64
#define TLV_RESTORE_QUEUE_ELM   0x1F65
#define TLV_RESTORE_TREE        0x1F66

#define RESTORE_BLOCK_HEIGHT    4
#define RESTORE_BLOCK_CHILD     16
#define RESTORE_BLOCK_BIN       4
#define RESTORE_BLOCK_SIZE      (((1 << (RESTORE_BLOCK_BIN * RESTORE_BLOCK_HEIGHT)) - 1) / (RESTORE_BLOCK_CHILD - 1))

#endif //RESTORE_H
