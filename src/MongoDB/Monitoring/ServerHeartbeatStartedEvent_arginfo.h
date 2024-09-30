/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 7cb420cf908d682f770a9dbc20473e075af9ce53 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent_getPort, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent_getHost, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent_isAwaited, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent, __construct);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent, getPort);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent, getHost);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent, isAwaited);


static const zend_function_entry class_MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent_methods[] = {
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent, __construct, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent, getPort, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent_getPort, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent, getHost, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent_getHost, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent, isAwaited, arginfo_class_MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent_isAwaited, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "ServerHeartbeatStartedEvent", class_MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;

	return class_entry;
}
