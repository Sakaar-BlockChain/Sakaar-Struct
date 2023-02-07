#ifndef SMART_SAKAAR_H
#define SMART_SAKAAR_H

#include "struct.h"

#include "basic/account.h"
#include "basic/account_connections.h"
#include "basic/activated_accounts.h"
#include "basic/block.h"
#include "basic/block_history.h"
#include "basic/currency.h"
#include "basic/generation.h"
#include "basic/pre_transaction.h"
#include "basic/transaction.h"
#include "basic/wallet.h"
#include "basic/wallet_data.h"
#include "basic/wallet_smart.h"


#define TLV_ACCOUNT         0x1F30
#define TLV_ACCOUNT_CONN    0x5F31
#define TLV_ACTIVE_ACC      0x5F32
#define TLV_BLOCK           0x1F40
#define TLV_BLOCK_HISTORY   0x1F41
#define TLV_CURRENCY        0x1F10
#define TLV_GENERATION      0x1F50
#define TLV_TRANSACTION     0x1F50
#define TLV_WALLET          0x1F20
#define TLV_WALLET_DATA     0x1F21
#define TLV_WALLET_SMART    0x1F22



#define ACCOUNT_SIZE            sizeof(struct account_st)
#define ACCOUNT_CONN_SIZE       sizeof(struct account_connections)
#define ACTIVE_ACC_SIZE         sizeof(struct activated_accounts)
#define BLOCK_SIZE              sizeof(struct block_st)
#define BLOCK_HISTORY_SIZE      sizeof(struct block_history)
#define CURRENCY_SIZE           sizeof(struct currency_st)
#define GENERATION_SIZE         sizeof(struct currency_st)
#define PRE_TRANSACTION_SIZE    sizeof(struct currency_st)
#define TRANSACTION_SIZE        sizeof(struct transaction_st)
#define WALLET_SIZE             sizeof(struct wallet_st)
#define WALLET_DATA_SIZE        sizeof(struct wallet_data)
#define WALLET_SMART_SIZE       sizeof(struct wallet_smart)


#define ACCOUNT_OP          (struct object_op) {METHOD_NEW &account_new, METHOD_SET &account_set, METHOD_CLEAR &account_clear, METHOD_FREE &account_free, METHOD_CMP NULL}
#define ACCOUNT_CONN_OP     (struct object_op) {METHOD_NEW &account_connections_new, METHOD_SET &account_connections_set, METHOD_CLEAR &account_connections_clear, METHOD_FREE &account_connections_free, METHOD_CMP NULL}
#define ACTIVE_ACC_OP       (struct object_op) {METHOD_NEW &activated_accounts_new, METHOD_SET &activated_accounts_set, METHOD_CLEAR &activated_accounts_clear, METHOD_FREE &activated_accounts_free, METHOD_CMP NULL}
#define BLOCK_OP            (struct object_op) {METHOD_NEW &block_new, METHOD_SET &block_set, METHOD_CLEAR &block_clear, METHOD_FREE &block_free, NULL}
#define BLOCK_HISTORY_OP    (struct object_op) {METHOD_NEW &block_history_new, METHOD_SET &block_history_set, METHOD_CLEAR &block_history_clear, METHOD_FREE &block_history_free, NULL}
#define CURRENCY_OP         (struct object_op) {METHOD_NEW &currency_new, METHOD_SET &currency_set, METHOD_CLEAR &currency_clear, METHOD_FREE &currency_free, NULL}
#define GENERATION_OP       (struct object_op) {METHOD_NEW &generation_new, METHOD_SET &generation_set, METHOD_CLEAR &generation_clear, METHOD_FREE &generation_free, NULL}
#define PRE_TRANSACTION_OP  (struct object_op) {METHOD_NEW &pre_transaction_new, METHOD_SET &pre_transaction_set, METHOD_CLEAR &pre_transaction_clear, METHOD_FREE &pre_transaction_free, NULL}
#define TRANSACTION_OP      (struct object_op) {METHOD_NEW &transaction_new, METHOD_SET &transaction_set, METHOD_CLEAR &transaction_clear, METHOD_FREE &transaction_free, NULL}
#define WALLET_OP           (struct object_op) {METHOD_NEW &wallet_new, METHOD_SET &wallet_set, METHOD_CLEAR &wallet_clear, METHOD_FREE &wallet_free, NULL}
#define WALLET_DATA_OP      (struct object_op) {METHOD_NEW &wallet_data_new, METHOD_SET &wallet_data_set, METHOD_CLEAR &wallet_data_clear, METHOD_FREE &wallet_data_free, NULL}
#define WALLET_SMART_OP     (struct object_op) {METHOD_NEW &wallet_smart_new, METHOD_SET &wallet_smart_set, METHOD_CLEAR &wallet_smart_clear, METHOD_FREE &wallet_smart_free, NULL}


extern struct object_type account_type;
extern struct object_type account_conn_type;
extern struct object_type active_acc_type;
extern struct object_type block_type;
extern struct object_type block_history_type;
extern struct object_type currency_type;
extern struct object_type generation_type;
extern struct object_type pre_transaction_type;
extern struct object_type transaction_type;
extern struct object_type wallet_type;
extern struct object_type wallet_data_type;
extern struct object_type wallet_smart_type;


#define ACCOUNT_TYPE            &account_type
#define ACCOUNT_CONN_TYPE       &account_conn_type
#define ACTIVE_ACC_TYPE         &active_acc_type
#define BLOCK_TYPE              &block_type
#define BLOCK_HISTORY_TYPE      &block_history_type
#define CURRENCY_TYPE           &currency_type
#define GENERATION_TYPE         &generation_type
#define PRE_TRANSACTION_TYPE    &pre_transaction_type
#define TRANSACTION_TYPE        &transaction_type
#define WALLET_TYPE             &wallet_type
#define WALLET_DATA_TYPE        &wallet_data_type
#define WALLET_SMART_TYPE       &wallet_smart_type

#endif //SMART_SAKAAR_H
