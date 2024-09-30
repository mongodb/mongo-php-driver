/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 22e0a387ed21d232c61f805ba258c4781a08a68e */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Session___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Session_abortTransaction, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Session_advanceClusterTime, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_MASK(0, clusterTime, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Session_advanceOperationTime, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, operationTime, MongoDB\\BSON\\TimestampInterface, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Session_commitTransaction arginfo_class_MongoDB_Driver_Session_abortTransaction

#define arginfo_class_MongoDB_Driver_Session_endSession arginfo_class_MongoDB_Driver_Session_abortTransaction

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Session_getClusterTime, 0, 0, IS_OBJECT, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Session_getLogicalSessionId, 0, 0, IS_OBJECT, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Session_getOperationTime, 0, 0, MongoDB\\BSON\\Timestamp, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Session_getServer, 0, 0, MongoDB\\Driver\\Server, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Session_getTransactionOptions, 0, 0, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Session_getTransactionState, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Session_isDirty, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Session_isInTransaction arginfo_class_MongoDB_Driver_Session_isDirty

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Session_startTransaction, 0, 0, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_Session, __construct);
static ZEND_METHOD(MongoDB_Driver_Session, abortTransaction);
static ZEND_METHOD(MongoDB_Driver_Session, advanceClusterTime);
static ZEND_METHOD(MongoDB_Driver_Session, advanceOperationTime);
static ZEND_METHOD(MongoDB_Driver_Session, commitTransaction);
static ZEND_METHOD(MongoDB_Driver_Session, endSession);
static ZEND_METHOD(MongoDB_Driver_Session, getClusterTime);
static ZEND_METHOD(MongoDB_Driver_Session, getLogicalSessionId);
static ZEND_METHOD(MongoDB_Driver_Session, getOperationTime);
static ZEND_METHOD(MongoDB_Driver_Session, getServer);
static ZEND_METHOD(MongoDB_Driver_Session, getTransactionOptions);
static ZEND_METHOD(MongoDB_Driver_Session, getTransactionState);
static ZEND_METHOD(MongoDB_Driver_Session, isDirty);
static ZEND_METHOD(MongoDB_Driver_Session, isInTransaction);
static ZEND_METHOD(MongoDB_Driver_Session, startTransaction);


static const zend_function_entry class_MongoDB_Driver_Session_methods[] = {
	ZEND_ME(MongoDB_Driver_Session, __construct, arginfo_class_MongoDB_Driver_Session___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Session, abortTransaction, arginfo_class_MongoDB_Driver_Session_abortTransaction, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Session, advanceClusterTime, arginfo_class_MongoDB_Driver_Session_advanceClusterTime, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Session, advanceOperationTime, arginfo_class_MongoDB_Driver_Session_advanceOperationTime, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Session, commitTransaction, arginfo_class_MongoDB_Driver_Session_commitTransaction, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Session, endSession, arginfo_class_MongoDB_Driver_Session_endSession, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Session, getClusterTime, arginfo_class_MongoDB_Driver_Session_getClusterTime, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Session, getLogicalSessionId, arginfo_class_MongoDB_Driver_Session_getLogicalSessionId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Session, getOperationTime, arginfo_class_MongoDB_Driver_Session_getOperationTime, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Session, getServer, arginfo_class_MongoDB_Driver_Session_getServer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Session, getTransactionOptions, arginfo_class_MongoDB_Driver_Session_getTransactionOptions, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Session, getTransactionState, arginfo_class_MongoDB_Driver_Session_getTransactionState, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Session, isDirty, arginfo_class_MongoDB_Driver_Session_isDirty, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Session, isInTransaction, arginfo_class_MongoDB_Driver_Session_isInTransaction, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Session, startTransaction, arginfo_class_MongoDB_Driver_Session_startTransaction, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Session(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Session", class_MongoDB_Driver_Session_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;

	zval const_TRANSACTION_NONE_value;
	zend_string *const_TRANSACTION_NONE_value_str = zend_string_init(PHONGO_TRANSACTION_NONE, strlen(PHONGO_TRANSACTION_NONE), 1);
	ZVAL_STR(&const_TRANSACTION_NONE_value, const_TRANSACTION_NONE_value_str);
	zend_string *const_TRANSACTION_NONE_name = zend_string_init_interned("TRANSACTION_NONE", sizeof("TRANSACTION_NONE") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TRANSACTION_NONE_name, &const_TRANSACTION_NONE_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TRANSACTION_NONE_name);

	zval const_TRANSACTION_STARTING_value;
	zend_string *const_TRANSACTION_STARTING_value_str = zend_string_init(PHONGO_TRANSACTION_STARTING, strlen(PHONGO_TRANSACTION_STARTING), 1);
	ZVAL_STR(&const_TRANSACTION_STARTING_value, const_TRANSACTION_STARTING_value_str);
	zend_string *const_TRANSACTION_STARTING_name = zend_string_init_interned("TRANSACTION_STARTING", sizeof("TRANSACTION_STARTING") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TRANSACTION_STARTING_name, &const_TRANSACTION_STARTING_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TRANSACTION_STARTING_name);

	zval const_TRANSACTION_IN_PROGRESS_value;
	zend_string *const_TRANSACTION_IN_PROGRESS_value_str = zend_string_init(PHONGO_TRANSACTION_IN_PROGRESS, strlen(PHONGO_TRANSACTION_IN_PROGRESS), 1);
	ZVAL_STR(&const_TRANSACTION_IN_PROGRESS_value, const_TRANSACTION_IN_PROGRESS_value_str);
	zend_string *const_TRANSACTION_IN_PROGRESS_name = zend_string_init_interned("TRANSACTION_IN_PROGRESS", sizeof("TRANSACTION_IN_PROGRESS") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TRANSACTION_IN_PROGRESS_name, &const_TRANSACTION_IN_PROGRESS_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TRANSACTION_IN_PROGRESS_name);

	zval const_TRANSACTION_COMMITTED_value;
	zend_string *const_TRANSACTION_COMMITTED_value_str = zend_string_init(PHONGO_TRANSACTION_COMMITTED, strlen(PHONGO_TRANSACTION_COMMITTED), 1);
	ZVAL_STR(&const_TRANSACTION_COMMITTED_value, const_TRANSACTION_COMMITTED_value_str);
	zend_string *const_TRANSACTION_COMMITTED_name = zend_string_init_interned("TRANSACTION_COMMITTED", sizeof("TRANSACTION_COMMITTED") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TRANSACTION_COMMITTED_name, &const_TRANSACTION_COMMITTED_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TRANSACTION_COMMITTED_name);

	zval const_TRANSACTION_ABORTED_value;
	zend_string *const_TRANSACTION_ABORTED_value_str = zend_string_init(PHONGO_TRANSACTION_ABORTED, strlen(PHONGO_TRANSACTION_ABORTED), 1);
	ZVAL_STR(&const_TRANSACTION_ABORTED_value, const_TRANSACTION_ABORTED_value_str);
	zend_string *const_TRANSACTION_ABORTED_name = zend_string_init_interned("TRANSACTION_ABORTED", sizeof("TRANSACTION_ABORTED") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TRANSACTION_ABORTED_name, &const_TRANSACTION_ABORTED_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TRANSACTION_ABORTED_name);

	return class_entry;
}
