/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 2d55d85a35a4a21004611412546aeab7ffdbad93 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent_getDurationMicros, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent_getError, 0, 0, Exception, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent_getPort arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent_getDurationMicros

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent_getHost, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent_isAwaited, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, __construct);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getDurationMicros);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getError);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getPort);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getHost);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, isAwaited);

static const zend_function_entry class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent_methods[] = {
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, __construct, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getDurationMicros, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent_getDurationMicros, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getError, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent_getError, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getPort, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent_getPort, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getHost, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent_getHost, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, isAwaited, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent_isAwaited, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "ServerHeartbeatFailedEvent", class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent_methods);
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

	zval property_awaited_default_value;
	ZVAL_UNDEF(&property_awaited_default_value);
	zend_string *property_awaited_name = zend_string_init("awaited", sizeof("awaited") - 1, 1);
	zend_declare_typed_property(class_entry, property_awaited_name, &property_awaited_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release(property_awaited_name);

	zval property_duration_default_value;
	ZVAL_UNDEF(&property_duration_default_value);
	zend_string *property_duration_name = zend_string_init("duration", sizeof("duration") - 1, 1);
	zend_declare_typed_property(class_entry, property_duration_name, &property_duration_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_duration_name);

	zval property_error_default_value;
	ZVAL_UNDEF(&property_error_default_value);
	zend_string *property_error_name = zend_string_init("error", sizeof("error") - 1, 1);
	zend_string *property_error_class_Exception = zend_string_init("Exception", sizeof("Exception")-1, 1);
	zend_declare_typed_property(class_entry, property_error_name, &property_error_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_error_class_Exception, 0, 0));
	zend_string_release(property_error_name);

	return class_entry;
}
