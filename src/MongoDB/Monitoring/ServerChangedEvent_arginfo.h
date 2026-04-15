/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 978047b8c3560beeb59e80be3d9c132e3b9d2c93 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerChangedEvent___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerChangedEvent_getPort, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerChangedEvent_getHost, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerChangedEvent_getNewDescription, 0, 0, MongoDB\\Driver\\ServerDescription, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Monitoring_ServerChangedEvent_getPreviousDescription arginfo_class_MongoDB_Driver_Monitoring_ServerChangedEvent_getNewDescription

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerChangedEvent_getTopologyId, 0, 0, MongoDB\\BSON\\ObjectId, 0)
ZEND_END_ARG_INFO()

static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerChangedEvent, __construct);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerChangedEvent, getPort);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerChangedEvent, getHost);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerChangedEvent, getNewDescription);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerChangedEvent, getPreviousDescription);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerChangedEvent, getTopologyId);

static const zend_function_entry class_MongoDB_Driver_Monitoring_ServerChangedEvent_methods[] = {
	ZEND_ME(MongoDB_Driver_Monitoring_ServerChangedEvent, __construct, arginfo_class_MongoDB_Driver_Monitoring_ServerChangedEvent___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerChangedEvent, getPort, arginfo_class_MongoDB_Driver_Monitoring_ServerChangedEvent_getPort, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerChangedEvent, getHost, arginfo_class_MongoDB_Driver_Monitoring_ServerChangedEvent_getHost, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerChangedEvent, getNewDescription, arginfo_class_MongoDB_Driver_Monitoring_ServerChangedEvent_getNewDescription, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerChangedEvent, getPreviousDescription, arginfo_class_MongoDB_Driver_Monitoring_ServerChangedEvent_getPreviousDescription, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerChangedEvent, getTopologyId, arginfo_class_MongoDB_Driver_Monitoring_ServerChangedEvent_getTopologyId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Monitoring_ServerChangedEvent(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "ServerChangedEvent", class_MongoDB_Driver_Monitoring_ServerChangedEvent_methods);
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

	zval property_topologyId_default_value;
	ZVAL_UNDEF(&property_topologyId_default_value);
	zend_string *property_topologyId_name = zend_string_init("topologyId", sizeof("topologyId") - 1, 1);
	zend_string *property_topologyId_class_MongoDB_BSON_ObjectId = zend_string_init("MongoDB\\BSON\\ObjectId", sizeof("MongoDB\\BSON\\ObjectId")-1, 1);
	zend_declare_typed_property(class_entry, property_topologyId_name, &property_topologyId_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_topologyId_class_MongoDB_BSON_ObjectId, 0, 0));
	zend_string_release(property_topologyId_name);

	zval property_newDescription_default_value;
	ZVAL_UNDEF(&property_newDescription_default_value);
	zend_string *property_newDescription_name = zend_string_init("newDescription", sizeof("newDescription") - 1, 1);
	zend_string *property_newDescription_class_MongoDB_Driver_ServerDescription = zend_string_init("MongoDB\\Driver\\ServerDescription", sizeof("MongoDB\\Driver\\ServerDescription")-1, 1);
	zend_declare_typed_property(class_entry, property_newDescription_name, &property_newDescription_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_newDescription_class_MongoDB_Driver_ServerDescription, 0, 0));
	zend_string_release(property_newDescription_name);

	zval property_previousDescription_default_value;
	ZVAL_UNDEF(&property_previousDescription_default_value);
	zend_string *property_previousDescription_name = zend_string_init("previousDescription", sizeof("previousDescription") - 1, 1);
	zend_string *property_previousDescription_class_MongoDB_Driver_ServerDescription = zend_string_init("MongoDB\\Driver\\ServerDescription", sizeof("MongoDB\\Driver\\ServerDescription")-1, 1);
	zend_declare_typed_property(class_entry, property_previousDescription_name, &property_previousDescription_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_previousDescription_class_MongoDB_Driver_ServerDescription, 0, 0));
	zend_string_release(property_previousDescription_name);

	return class_entry;
}
