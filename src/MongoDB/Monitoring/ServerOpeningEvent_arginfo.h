/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: e05cac5a6840d453e7c00175763fc8d1b659054f */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerOpeningEvent___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerOpeningEvent_getPort, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerOpeningEvent_getHost, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerOpeningEvent_getTopologyId, 0, 0, MongoDB\\BSON\\ObjectId, 0)
ZEND_END_ARG_INFO()

static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerOpeningEvent, __construct);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerOpeningEvent, getPort);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerOpeningEvent, getHost);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerOpeningEvent, getTopologyId);

static const zend_function_entry class_MongoDB_Driver_Monitoring_ServerOpeningEvent_methods[] = {
	ZEND_ME(MongoDB_Driver_Monitoring_ServerOpeningEvent, __construct, arginfo_class_MongoDB_Driver_Monitoring_ServerOpeningEvent___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerOpeningEvent, getPort, arginfo_class_MongoDB_Driver_Monitoring_ServerOpeningEvent_getPort, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerOpeningEvent, getHost, arginfo_class_MongoDB_Driver_Monitoring_ServerOpeningEvent_getHost, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerOpeningEvent, getTopologyId, arginfo_class_MongoDB_Driver_Monitoring_ServerOpeningEvent_getTopologyId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Monitoring_ServerOpeningEvent(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "ServerOpeningEvent", class_MongoDB_Driver_Monitoring_ServerOpeningEvent_methods);
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

	return class_entry;
}
