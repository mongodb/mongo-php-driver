/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 5edd7242100364c5e4beddc89b4d92dda70c09f6 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Server___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Server_executeBulkWrite, 0, 2, MongoDB\\Driver\\WriteResult, 0)
	ZEND_ARG_TYPE_INFO(0, namespace, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, bulkWrite, MongoDB\\Driver\\BulkWrite, 0)
	ZEND_ARG_OBJ_TYPE_MASK(0, options, MongoDB\\Driver\\WriteConcern, MAY_BE_ARRAY|MAY_BE_NULL, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Server_executeCommand, 0, 2, MongoDB\\Driver\\Cursor, 0)
	ZEND_ARG_TYPE_INFO(0, db, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Driver\\Command, 0)
	ZEND_ARG_OBJ_TYPE_MASK(0, options, MongoDB\\Driver\\ReadPreference, MAY_BE_ARRAY|MAY_BE_NULL, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Server_executeQuery, 0, 2, MongoDB\\Driver\\Cursor, 0)
	ZEND_ARG_TYPE_INFO(0, namespace, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, query, MongoDB\\Driver\\Query, 0)
	ZEND_ARG_OBJ_TYPE_MASK(0, options, MongoDB\\Driver\\ReadPreference, MAY_BE_ARRAY|MAY_BE_NULL, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Server_executeReadCommand, 0, 2, MongoDB\\Driver\\Cursor, 0)
	ZEND_ARG_TYPE_INFO(0, db, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Driver\\Command, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Server_executeReadWriteCommand arginfo_class_MongoDB_Driver_Server_executeReadCommand

#define arginfo_class_MongoDB_Driver_Server_executeWriteCommand arginfo_class_MongoDB_Driver_Server_executeReadCommand

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Server_getHost, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Server_getInfo, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Server_getLatency, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Server_getPort, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Server_getServerDescription, 0, 0, MongoDB\\Driver\\ServerDescription, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Server_getTags arginfo_class_MongoDB_Driver_Server_getInfo

#define arginfo_class_MongoDB_Driver_Server_getType arginfo_class_MongoDB_Driver_Server_getPort

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Server_isArbiter, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Server_isHidden arginfo_class_MongoDB_Driver_Server_isArbiter

#define arginfo_class_MongoDB_Driver_Server_isPassive arginfo_class_MongoDB_Driver_Server_isArbiter

#define arginfo_class_MongoDB_Driver_Server_isPrimary arginfo_class_MongoDB_Driver_Server_isArbiter

#define arginfo_class_MongoDB_Driver_Server_isSecondary arginfo_class_MongoDB_Driver_Server_isArbiter


static ZEND_METHOD(MongoDB_Driver_Server, __construct);
static ZEND_METHOD(MongoDB_Driver_Server, executeBulkWrite);
static ZEND_METHOD(MongoDB_Driver_Server, executeCommand);
static ZEND_METHOD(MongoDB_Driver_Server, executeQuery);
static ZEND_METHOD(MongoDB_Driver_Server, executeReadCommand);
static ZEND_METHOD(MongoDB_Driver_Server, executeReadWriteCommand);
static ZEND_METHOD(MongoDB_Driver_Server, executeWriteCommand);
static ZEND_METHOD(MongoDB_Driver_Server, getHost);
static ZEND_METHOD(MongoDB_Driver_Server, getInfo);
static ZEND_METHOD(MongoDB_Driver_Server, getLatency);
static ZEND_METHOD(MongoDB_Driver_Server, getPort);
static ZEND_METHOD(MongoDB_Driver_Server, getServerDescription);
static ZEND_METHOD(MongoDB_Driver_Server, getTags);
static ZEND_METHOD(MongoDB_Driver_Server, getType);
static ZEND_METHOD(MongoDB_Driver_Server, isArbiter);
static ZEND_METHOD(MongoDB_Driver_Server, isHidden);
static ZEND_METHOD(MongoDB_Driver_Server, isPassive);
static ZEND_METHOD(MongoDB_Driver_Server, isPrimary);
static ZEND_METHOD(MongoDB_Driver_Server, isSecondary);


static const zend_function_entry class_MongoDB_Driver_Server_methods[] = {
	ZEND_ME(MongoDB_Driver_Server, __construct, arginfo_class_MongoDB_Driver_Server___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, executeBulkWrite, arginfo_class_MongoDB_Driver_Server_executeBulkWrite, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, executeCommand, arginfo_class_MongoDB_Driver_Server_executeCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, executeQuery, arginfo_class_MongoDB_Driver_Server_executeQuery, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, executeReadCommand, arginfo_class_MongoDB_Driver_Server_executeReadCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, executeReadWriteCommand, arginfo_class_MongoDB_Driver_Server_executeReadWriteCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, executeWriteCommand, arginfo_class_MongoDB_Driver_Server_executeWriteCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, getHost, arginfo_class_MongoDB_Driver_Server_getHost, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, getInfo, arginfo_class_MongoDB_Driver_Server_getInfo, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, getLatency, arginfo_class_MongoDB_Driver_Server_getLatency, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, getPort, arginfo_class_MongoDB_Driver_Server_getPort, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, getServerDescription, arginfo_class_MongoDB_Driver_Server_getServerDescription, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, getTags, arginfo_class_MongoDB_Driver_Server_getTags, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, getType, arginfo_class_MongoDB_Driver_Server_getType, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, isArbiter, arginfo_class_MongoDB_Driver_Server_isArbiter, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, isHidden, arginfo_class_MongoDB_Driver_Server_isHidden, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, isPassive, arginfo_class_MongoDB_Driver_Server_isPassive, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, isPrimary, arginfo_class_MongoDB_Driver_Server_isPrimary, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Server, isSecondary, arginfo_class_MongoDB_Driver_Server_isSecondary, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Server(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Server", class_MongoDB_Driver_Server_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;

	zval const_TYPE_UNKNOWN_value;
	ZVAL_LONG(&const_TYPE_UNKNOWN_value, PHONGO_SERVER_UNKNOWN);
	zend_string *const_TYPE_UNKNOWN_name = zend_string_init_interned("TYPE_UNKNOWN", sizeof("TYPE_UNKNOWN") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_UNKNOWN_name, &const_TYPE_UNKNOWN_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_UNKNOWN_name);

	zval const_TYPE_STANDALONE_value;
	ZVAL_LONG(&const_TYPE_STANDALONE_value, PHONGO_SERVER_STANDALONE);
	zend_string *const_TYPE_STANDALONE_name = zend_string_init_interned("TYPE_STANDALONE", sizeof("TYPE_STANDALONE") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_STANDALONE_name, &const_TYPE_STANDALONE_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_STANDALONE_name);

	zval const_TYPE_MONGOS_value;
	ZVAL_LONG(&const_TYPE_MONGOS_value, PHONGO_SERVER_MONGOS);
	zend_string *const_TYPE_MONGOS_name = zend_string_init_interned("TYPE_MONGOS", sizeof("TYPE_MONGOS") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_MONGOS_name, &const_TYPE_MONGOS_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_MONGOS_name);

	zval const_TYPE_POSSIBLE_PRIMARY_value;
	ZVAL_LONG(&const_TYPE_POSSIBLE_PRIMARY_value, PHONGO_SERVER_POSSIBLE_PRIMARY);
	zend_string *const_TYPE_POSSIBLE_PRIMARY_name = zend_string_init_interned("TYPE_POSSIBLE_PRIMARY", sizeof("TYPE_POSSIBLE_PRIMARY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_POSSIBLE_PRIMARY_name, &const_TYPE_POSSIBLE_PRIMARY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_POSSIBLE_PRIMARY_name);

	zval const_TYPE_RS_PRIMARY_value;
	ZVAL_LONG(&const_TYPE_RS_PRIMARY_value, PHONGO_SERVER_RS_PRIMARY);
	zend_string *const_TYPE_RS_PRIMARY_name = zend_string_init_interned("TYPE_RS_PRIMARY", sizeof("TYPE_RS_PRIMARY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_RS_PRIMARY_name, &const_TYPE_RS_PRIMARY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_RS_PRIMARY_name);

	zval const_TYPE_RS_SECONDARY_value;
	ZVAL_LONG(&const_TYPE_RS_SECONDARY_value, PHONGO_SERVER_RS_SECONDARY);
	zend_string *const_TYPE_RS_SECONDARY_name = zend_string_init_interned("TYPE_RS_SECONDARY", sizeof("TYPE_RS_SECONDARY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_RS_SECONDARY_name, &const_TYPE_RS_SECONDARY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_RS_SECONDARY_name);

	zval const_TYPE_RS_ARBITER_value;
	ZVAL_LONG(&const_TYPE_RS_ARBITER_value, PHONGO_SERVER_RS_ARBITER);
	zend_string *const_TYPE_RS_ARBITER_name = zend_string_init_interned("TYPE_RS_ARBITER", sizeof("TYPE_RS_ARBITER") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_RS_ARBITER_name, &const_TYPE_RS_ARBITER_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_RS_ARBITER_name);

	zval const_TYPE_RS_OTHER_value;
	ZVAL_LONG(&const_TYPE_RS_OTHER_value, PHONGO_SERVER_RS_OTHER);
	zend_string *const_TYPE_RS_OTHER_name = zend_string_init_interned("TYPE_RS_OTHER", sizeof("TYPE_RS_OTHER") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_RS_OTHER_name, &const_TYPE_RS_OTHER_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_RS_OTHER_name);

	zval const_TYPE_RS_GHOST_value;
	ZVAL_LONG(&const_TYPE_RS_GHOST_value, PHONGO_SERVER_RS_GHOST);
	zend_string *const_TYPE_RS_GHOST_name = zend_string_init_interned("TYPE_RS_GHOST", sizeof("TYPE_RS_GHOST") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_RS_GHOST_name, &const_TYPE_RS_GHOST_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_RS_GHOST_name);

	zval const_TYPE_LOAD_BALANCER_value;
	ZVAL_LONG(&const_TYPE_LOAD_BALANCER_value, PHONGO_SERVER_LOAD_BALANCER);
	zend_string *const_TYPE_LOAD_BALANCER_name = zend_string_init_interned("TYPE_LOAD_BALANCER", sizeof("TYPE_LOAD_BALANCER") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_LOAD_BALANCER_name, &const_TYPE_LOAD_BALANCER_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_LOAD_BALANCER_name);

	return class_entry;
}
