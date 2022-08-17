/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 4b4373da62f18f2c3ab6aae78a495ed84adbe09b */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Manager___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, uri, IS_STRING, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, uriOptions, IS_ARRAY, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, driverOptions, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Manager_addSubscriber, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, subscriber, MongoDB\\Driver\\Monitoring\\Subscriber, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Manager_createClientEncryption, 0, 1, MongoDB\\Driver\\ClientEncryption, 0)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Manager_executeBulkWrite, 0, 2, MongoDB\\Driver\\WriteResult, 0)
	ZEND_ARG_TYPE_INFO(0, namespace, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, bulk, MongoDB\\Driver\\BulkWrite, 0)
	ZEND_ARG_OBJ_TYPE_MASK(0, options, MongoDB\\Driver\\WriteConcern, MAY_BE_ARRAY|MAY_BE_NULL, "null")
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Manager_executeBulkWrite, 0, 2, MongoDB\\Driver\\WriteResult, 0)
	ZEND_ARG_TYPE_INFO(0, namespace, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, bulk, MongoDB\\Driver\\BulkWrite, 0)
	ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, options, "null")
ZEND_END_ARG_INFO()
#endif

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Manager_executeCommand, 0, 2, MongoDB\\Driver\\Cursor, 0)
	ZEND_ARG_TYPE_INFO(0, db, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Driver\\Command, 0)
	ZEND_ARG_OBJ_TYPE_MASK(0, options, MongoDB\\Driver\\ReadPreference, MAY_BE_ARRAY|MAY_BE_NULL, "null")
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Manager_executeCommand, 0, 2, MongoDB\\Driver\\Cursor, 0)
	ZEND_ARG_TYPE_INFO(0, db, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Driver\\Command, 0)
	ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, options, "null")
ZEND_END_ARG_INFO()
#endif

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Manager_executeQuery, 0, 2, MongoDB\\Driver\\Cursor, 0)
	ZEND_ARG_TYPE_INFO(0, namespace, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, query, MongoDB\\Driver\\Query, 0)
	ZEND_ARG_OBJ_TYPE_MASK(0, options, MongoDB\\Driver\\ReadPreference, MAY_BE_ARRAY|MAY_BE_NULL, "null")
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Manager_executeQuery, 0, 2, MongoDB\\Driver\\Cursor, 0)
	ZEND_ARG_TYPE_INFO(0, namespace, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, query, MongoDB\\Driver\\Query, 0)
	ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, options, "null")
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Manager_executeReadCommand, 0, 2, MongoDB\\Driver\\Cursor, 0)
	ZEND_ARG_TYPE_INFO(0, db, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Driver\\Command, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Manager_executeReadWriteCommand arginfo_class_MongoDB_Driver_Manager_executeReadCommand

#define arginfo_class_MongoDB_Driver_Manager_executeWriteCommand arginfo_class_MongoDB_Driver_Manager_executeReadCommand

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_MongoDB_Driver_Manager_getEncryptedFieldsMap, 0, 0, MAY_BE_ARRAY|MAY_BE_OBJECT|MAY_BE_NULL)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Manager_getEncryptedFieldsMap, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Manager_getReadConcern, 0, 0, MongoDB\\Driver\\ReadConcern, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Manager_getReadPreference, 0, 0, MongoDB\\Driver\\ReadPreference, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Manager_getServers, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Manager_getWriteConcern, 0, 0, MongoDB\\Driver\\WriteConcern, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Manager_removeSubscriber arginfo_class_MongoDB_Driver_Manager_addSubscriber

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Manager_selectServer, 0, 0, MongoDB\\Driver\\Server, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, readPreference, MongoDB\\Driver\\ReadPreference, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Manager_startSession, 0, 0, MongoDB\\Driver\\Session, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Manager___wakeup, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_Manager, __construct);
static ZEND_METHOD(MongoDB_Driver_Manager, addSubscriber);
static ZEND_METHOD(MongoDB_Driver_Manager, createClientEncryption);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_Driver_Manager, executeBulkWrite);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_Driver_Manager, executeBulkWrite);
#endif
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_Driver_Manager, executeCommand);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_Driver_Manager, executeCommand);
#endif
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_Driver_Manager, executeQuery);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_Driver_Manager, executeQuery);
#endif
static ZEND_METHOD(MongoDB_Driver_Manager, executeReadCommand);
static ZEND_METHOD(MongoDB_Driver_Manager, executeReadWriteCommand);
static ZEND_METHOD(MongoDB_Driver_Manager, executeWriteCommand);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_Driver_Manager, getEncryptedFieldsMap);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_Driver_Manager, getEncryptedFieldsMap);
#endif
static ZEND_METHOD(MongoDB_Driver_Manager, getReadConcern);
static ZEND_METHOD(MongoDB_Driver_Manager, getReadPreference);
static ZEND_METHOD(MongoDB_Driver_Manager, getServers);
static ZEND_METHOD(MongoDB_Driver_Manager, getWriteConcern);
static ZEND_METHOD(MongoDB_Driver_Manager, removeSubscriber);
static ZEND_METHOD(MongoDB_Driver_Manager, selectServer);
static ZEND_METHOD(MongoDB_Driver_Manager, startSession);
static ZEND_METHOD(MongoDB_Driver_Manager, __wakeup);


static const zend_function_entry class_MongoDB_Driver_Manager_methods[] = {
	ZEND_ME(MongoDB_Driver_Manager, __construct, arginfo_class_MongoDB_Driver_Manager___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Manager, addSubscriber, arginfo_class_MongoDB_Driver_Manager_addSubscriber, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Manager, createClientEncryption, arginfo_class_MongoDB_Driver_Manager_createClientEncryption, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_Driver_Manager, executeBulkWrite, arginfo_class_MongoDB_Driver_Manager_executeBulkWrite, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_Driver_Manager, executeBulkWrite, arginfo_class_MongoDB_Driver_Manager_executeBulkWrite, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_Driver_Manager, executeCommand, arginfo_class_MongoDB_Driver_Manager_executeCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_Driver_Manager, executeCommand, arginfo_class_MongoDB_Driver_Manager_executeCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_Driver_Manager, executeQuery, arginfo_class_MongoDB_Driver_Manager_executeQuery, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_Driver_Manager, executeQuery, arginfo_class_MongoDB_Driver_Manager_executeQuery, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_Driver_Manager, executeReadCommand, arginfo_class_MongoDB_Driver_Manager_executeReadCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Manager, executeReadWriteCommand, arginfo_class_MongoDB_Driver_Manager_executeReadWriteCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Manager, executeWriteCommand, arginfo_class_MongoDB_Driver_Manager_executeWriteCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_Driver_Manager, getEncryptedFieldsMap, arginfo_class_MongoDB_Driver_Manager_getEncryptedFieldsMap, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_Driver_Manager, getEncryptedFieldsMap, arginfo_class_MongoDB_Driver_Manager_getEncryptedFieldsMap, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_Driver_Manager, getReadConcern, arginfo_class_MongoDB_Driver_Manager_getReadConcern, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Manager, getReadPreference, arginfo_class_MongoDB_Driver_Manager_getReadPreference, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Manager, getServers, arginfo_class_MongoDB_Driver_Manager_getServers, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Manager, getWriteConcern, arginfo_class_MongoDB_Driver_Manager_getWriteConcern, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Manager, removeSubscriber, arginfo_class_MongoDB_Driver_Manager_removeSubscriber, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Manager, selectServer, arginfo_class_MongoDB_Driver_Manager_selectServer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Manager, startSession, arginfo_class_MongoDB_Driver_Manager_startSession, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Manager, __wakeup, arginfo_class_MongoDB_Driver_Manager___wakeup, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Manager(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Manager", class_MongoDB_Driver_Manager_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}
