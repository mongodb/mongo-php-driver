/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 7fccc6765f058925077bb698c8a79ddb8355b852 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent_getDurationMicros, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent_getReply, 0, 0, IS_OBJECT, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent_getPort arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent_getDurationMicros

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent_getHost, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent_isAwaited, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, __construct);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getDurationMicros);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getReply);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getPort);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getHost);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, isAwaited);

static const zend_function_entry class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent_methods[] = {
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, __construct, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getDurationMicros, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent_getDurationMicros, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getReply, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent_getReply, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getPort, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent_getPort, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getHost, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent_getHost, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, isAwaited, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent_isAwaited, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "ServerHeartbeatSucceededEvent", class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent_methods);
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

	zval property_reply_default_value;
	ZVAL_UNDEF(&property_reply_default_value);
	zend_string *property_reply_name = zend_string_init("reply", sizeof("reply") - 1, 1);
	zend_declare_typed_property(class_entry, property_reply_name, &property_reply_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_OBJECT));
	zend_string_release(property_reply_name);

	return class_entry;
}
