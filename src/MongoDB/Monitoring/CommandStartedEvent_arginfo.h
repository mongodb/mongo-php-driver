/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: e250ffaa013fd197fb3946669d1f186da158606d */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getCommand, 0, 0, IS_OBJECT, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getCommandName, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getDatabaseName arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getCommandName

#define arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getHost arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getCommandName

#define arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getOperationId arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getCommandName

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getPort, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getRequestId arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getCommandName

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getServiceId, 0, 0, MongoDB\\BSON\\ObjectId, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getServerConnectionId, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO()

static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, __construct);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getCommand);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getCommandName);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getDatabaseName);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getHost);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getOperationId);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getPort);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getRequestId);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getServiceId);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getServerConnectionId);

static const zend_function_entry class_MongoDB_Driver_Monitoring_CommandStartedEvent_methods[] = {
	ZEND_ME(MongoDB_Driver_Monitoring_CommandStartedEvent, __construct, arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandStartedEvent, getCommand, arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandStartedEvent, getCommandName, arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getCommandName, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandStartedEvent, getDatabaseName, arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getDatabaseName, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandStartedEvent, getHost, arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getHost, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandStartedEvent, getOperationId, arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getOperationId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandStartedEvent, getPort, arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getPort, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandStartedEvent, getRequestId, arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getRequestId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandStartedEvent, getServiceId, arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getServiceId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandStartedEvent, getServerConnectionId, arginfo_class_MongoDB_Driver_Monitoring_CommandStartedEvent_getServerConnectionId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Monitoring_CommandStartedEvent(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "CommandStartedEvent", class_MongoDB_Driver_Monitoring_CommandStartedEvent_methods);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);
#else
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;
#endif

	zval property_host_default_value;
	ZVAL_UNDEF(&property_host_default_value);
	zend_declare_typed_property(class_entry, ZSTR_KNOWN(ZEND_STR_HOST), &property_host_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));

	zval property_port_default_value;
	ZVAL_UNDEF(&property_port_default_value);
	zend_declare_typed_property(class_entry, ZSTR_KNOWN(ZEND_STR_PORT), &property_port_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));

	zval property_commandName_default_value;
	ZVAL_UNDEF(&property_commandName_default_value);
	zend_string *property_commandName_name = zend_string_init("commandName", sizeof("commandName") - 1, 1);
	zend_declare_typed_property(class_entry, property_commandName_name, &property_commandName_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
	zend_string_release(property_commandName_name);

	zval property_databaseName_default_value;
	ZVAL_UNDEF(&property_databaseName_default_value);
	zend_string *property_databaseName_name = zend_string_init("databaseName", sizeof("databaseName") - 1, 1);
	zend_declare_typed_property(class_entry, property_databaseName_name, &property_databaseName_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
	zend_string_release(property_databaseName_name);

	zval property_command_default_value;
	ZVAL_UNDEF(&property_command_default_value);
	zend_string *property_command_name = zend_string_init("command", sizeof("command") - 1, 1);
	zend_declare_typed_property(class_entry, property_command_name, &property_command_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_OBJECT));
	zend_string_release(property_command_name);

	zval property_operationId_default_value;
	ZVAL_UNDEF(&property_operationId_default_value);
	zend_string *property_operationId_name = zend_string_init("operationId", sizeof("operationId") - 1, 1);
	zend_declare_typed_property(class_entry, property_operationId_name, &property_operationId_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
	zend_string_release(property_operationId_name);

	zval property_requestId_default_value;
	ZVAL_UNDEF(&property_requestId_default_value);
	zend_string *property_requestId_name = zend_string_init("requestId", sizeof("requestId") - 1, 1);
	zend_declare_typed_property(class_entry, property_requestId_name, &property_requestId_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
	zend_string_release(property_requestId_name);

	zval property_serviceId_default_value;
	ZVAL_UNDEF(&property_serviceId_default_value);
	zend_string *property_serviceId_name = zend_string_init("serviceId", sizeof("serviceId") - 1, 1);
	zend_string *property_serviceId_class_MongoDB_BSON_ObjectId = zend_string_init("MongoDB\\BSON\\ObjectId", sizeof("MongoDB\\BSON\\ObjectId")-1, 1);
	zend_declare_typed_property(class_entry, property_serviceId_name, &property_serviceId_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_serviceId_class_MongoDB_BSON_ObjectId, 0, MAY_BE_NULL));
	zend_string_release(property_serviceId_name);

	zval property_serverConnectionId_default_value;
	ZVAL_UNDEF(&property_serverConnectionId_default_value);
	zend_string *property_serverConnectionId_name = zend_string_init("serverConnectionId", sizeof("serverConnectionId") - 1, 1);
	zend_declare_typed_property(class_entry, property_serverConnectionId_name, &property_serverConnectionId_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release(property_serverConnectionId_name);

	return class_entry;
}
