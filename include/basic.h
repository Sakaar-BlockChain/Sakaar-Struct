#ifndef STRUCT_BASIC_H
#define STRUCT_BASIC_H

#include "struct.h"

#include "basic/account.h"
#include "basic/account_connections.h"
#include "basic/activated_accounts.h"
#include "basic/address_list.h"
#include "basic/block.h"
#include "basic/block_history.h"
#include "basic/currency.h"
#include "basic/generation.h"
#include "basic/hash_time.h"
#include "basic/hash_time_list.h"
#include "basic/packet_list.h"
#include "basic/pre_transaction.h"
#include "basic/transaction.h"
#include "basic/transaction_list.h"
#include "basic/smartcontract.h"
#include "basic/wallet.h"
#include "basic/wallet_data.h"
#include "basic/wallet_smart.h"


#define TLV_ACCOUNT         0x1F30
#define TLV_ACCOUNT_CONN    0x1F31
#define TLV_ACTIVE_ACC      0x1F32
#define TLV_ADDRESS_LIST    0x1F33
#define TLV_BLOCK           0x1F40
#define TLV_BLOCK_HISTORY   0x1F41
#define TLV_CURRENCY        0x1F10
#define TLV_GENERATION      0x1F50
#define TLV_HASH_TIME       0x1F42
#define TLV_HASH_TIME_LIST  0x1F43
#define TLV_PACKET_LIST     0x1F54
#define TLV_TRANSACTION     0x1F51
#define TLV_TRANS_LIST      0x1F52
#define TLV_SMARTCONTRACT   0x1F53
#define TLV_WALLET          0x1F20
#define TLV_WALLET_DATA     0x1F21
#define TLV_WALLET_SMART    0x1F22


#define ACCOUNT_OP          (struct object_op) {METHOD_NEW &account_new, METHOD_FREE &account_free, METHOD_SET &account_set, METHOD_SET &account_copy, NULL, NULL, METHOD_CLEAR &account_clear, METHOD_CMP &account_cmp}
#define ACCOUNT_CONN_OP     (struct object_op) {METHOD_NEW &account_connections_new, METHOD_FREE &account_connections_free, METHOD_SET &account_connections_set, METHOD_SET &account_connections_copy, NULL, NULL, METHOD_CLEAR &account_connections_clear, METHOD_CMP &account_connections_cmp}
#define ACTIVE_ACC_OP       (struct object_op) {METHOD_NEW &activated_accounts_new, METHOD_FREE &activated_accounts_free, METHOD_SET &activated_accounts_set, METHOD_SET &activated_accounts_copy, NULL, NULL, METHOD_CLEAR &activated_accounts_clear, METHOD_CMP &activated_accounts_cmp}
#define ADDRESS_LIST_OP     (struct object_op) {METHOD_NEW &address_list_new, METHOD_FREE &address_list_free, METHOD_SET &address_list_set, METHOD_SET &address_list_copy, NULL, NULL, METHOD_CLEAR &address_list_clear, METHOD_CMP &address_list_cmp}
#define BLOCK_OP            (struct object_op) {METHOD_NEW &block_new, METHOD_FREE &block_free, METHOD_SET &block_set, METHOD_SET &block_copy, NULL, NULL, METHOD_CLEAR &block_clear, METHOD_CMP &block_cmp}
#define BLOCK_HISTORY_OP    (struct object_op) {METHOD_NEW &block_history_new, METHOD_FREE &block_history_free, METHOD_SET &block_history_set, METHOD_SET &block_history_copy, NULL, NULL, METHOD_CLEAR &block_history_clear, METHOD_CMP &block_history_cmp}
#define CURRENCY_OP         (struct object_op) {METHOD_NEW &currency_new, METHOD_FREE &currency_free, METHOD_SET &currency_set, METHOD_SET &currency_copy, NULL, NULL, METHOD_CLEAR &currency_clear, METHOD_CMP &currency_cmp}
#define GENERATION_OP       (struct object_op) {METHOD_NEW &generation_new, METHOD_FREE &generation_free, METHOD_SET &generation_set, METHOD_SET &generation_copy, NULL, NULL, METHOD_CLEAR &generation_clear, METHOD_CMP &generation_cmp}
#define HASH_TIME_OP        (struct object_op) {METHOD_NEW &hash_time_new, METHOD_FREE &hash_time_free, METHOD_SET &hash_time_set, METHOD_SET &hash_time_copy, NULL, NULL, METHOD_CLEAR &hash_time_clear, METHOD_CMP &hash_time_cmp}
#define HASH_TIME_LIST_OP   (struct object_op) {METHOD_NEW &hash_time_list_new, METHOD_FREE &hash_time_list_free, METHOD_SET &hash_time_list_set, METHOD_SET &hash_time_list_copy, NULL, NULL, METHOD_CLEAR &hash_time_list_clear, METHOD_CMP &hash_time_list_cmp}
#define PACKET_LIST_OP      (struct object_op) {METHOD_NEW &packet_list_new, METHOD_FREE &packet_list_free, METHOD_SET &packet_list_set, METHOD_SET &packet_list_copy, NULL, NULL, METHOD_CLEAR &packet_list_clear, METHOD_CMP &packet_list_cmp}
#define PRE_TRANSACTION_OP  (struct object_op) {METHOD_NEW &pre_transaction_new, METHOD_FREE &pre_transaction_free, METHOD_SET &pre_transaction_set, METHOD_SET &pre_transaction_copy, NULL, NULL, METHOD_CLEAR &pre_transaction_clear, METHOD_CMP &pre_transaction_cmp}
#define TRANSACTION_OP      (struct object_op) {METHOD_NEW &transaction_new, METHOD_FREE &transaction_free, METHOD_SET &transaction_set, METHOD_SET &transaction_copy, NULL, NULL, METHOD_CLEAR &transaction_clear, METHOD_CMP &transaction_cmp}
#define TRANS_LIST_OP       (struct object_op) {METHOD_NEW &transaction_list_new, METHOD_FREE &transaction_list_free, METHOD_SET &transaction_list_set, METHOD_SET &transaction_list_copy, NULL, NULL, METHOD_CLEAR &transaction_list_clear, METHOD_CMP &transaction_list_cmp}
#define SMARTCONTRACT_OP    STRING_OP
#define WALLET_OP           (struct object_op) {METHOD_NEW &wallet_new, METHOD_FREE &wallet_free, METHOD_SET &wallet_set, METHOD_SET &wallet_copy, NULL, NULL, METHOD_CLEAR &wallet_clear, METHOD_CMP &wallet_cmp}
#define WALLET_DATA_OP      (struct object_op) {METHOD_NEW &wallet_data_new, METHOD_FREE &wallet_data_free, METHOD_SET &wallet_data_set, METHOD_SET &wallet_data_copy, NULL, NULL, METHOD_CLEAR &wallet_data_clear, METHOD_CMP &wallet_data_cmp}
#define WALLET_SMART_OP     (struct object_op) {METHOD_NEW &wallet_smart_new, METHOD_FREE &wallet_smart_free, METHOD_SET &wallet_smart_set, METHOD_SET &wallet_smart_copy, NULL, NULL, METHOD_CLEAR &wallet_smart_clear, METHOD_CMP &wallet_smart_cmp}


extern struct object_type account_type;
extern struct object_type account_conn_type;
extern struct object_type active_acc_type;
extern struct object_type address_list_type;
extern struct object_type block_type;
extern struct object_type block_history_type;
extern struct object_type currency_type;
extern struct object_type generation_type;
extern struct object_type hash_time_type;
extern struct object_type hash_time_list_type;
extern struct object_type packet_list_type;
extern struct object_type pre_transaction_type;
extern struct object_type transaction_type;
extern struct object_type transaction_list_type;
extern struct object_type smartcontract_type;
extern struct object_type wallet_type;
extern struct object_type wallet_data_type;
extern struct object_type wallet_smart_type;


#define ACCOUNT_TYPE            &account_type
#define ACCOUNT_CONN_TYPE       &account_conn_type
#define ACTIVE_ACC_TYPE         &active_acc_type
#define ADDRESS_LIST_TYPE       &address_list_type
#define BLOCK_TYPE              &block_type
#define BLOCK_HISTORY_TYPE      &block_history_type
#define CURRENCY_TYPE           &currency_type
#define GENERATION_TYPE         &generation_type
#define HASH_TIME_TYPE          &hash_time_type
#define HASH_TIME_LIST_TYPE     &hash_time_list_type
#define PACKET_LIST_TYPE        &packet_list_type
#define PRE_TRANSACTION_TYPE    &pre_transaction_type
#define TRANSACTION_TYPE        &transaction_type
#define TRANS_LIST_TYPE         &transaction_list_type
#define SMARTCONTRACT_TYPE      &smartcontract_type
#define WALLET_TYPE             &wallet_type
#define WALLET_DATA_TYPE        &wallet_data_type
#define WALLET_SMART_TYPE       &wallet_smart_type

#endif //STRUCT_BASIC_H
