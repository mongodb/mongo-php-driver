/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 05a48b4966fb49c82a6ef48b360a93fdc2919f77 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerClosedEvent___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerClosedEvent_getPort, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerClosedEvent_getHost, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_ServerClosedEvent_getTopologyId, 0, 0, MongoDB\\BSON\\ObjectId, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerClosedEvent, __construct);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerClosedEvent, getPort);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerClosedEvent, getHost);
static ZEND_METHOD(MongoDB_Driver_Monitoring_ServerClosedEvent, getTopologyId);


static const zend_function_entry class_MongoDB_Driver_Monitoring_ServerClosedEvent_methods[] = {
	ZEND_ME(MongoDB_Driver_Monitoring_ServerClosedEvent, __construct, arginfo_class_MongoDB_Driver_Monitoring_ServerClosedEvent___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerClosedEvent, getPort, arginfo_class_MongoDB_Driver_Monitoring_ServerClosedEvent_getPort, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerClosedEvent, getHost, arginfo_class_MongoDB_Driver_Monitoring_ServerClosedEvent_getHost, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_ServerClosedEvent, getTopologyId, arginfo_class_MongoDB_Driver_Monitoring_ServerClosedEvent_getTopologyId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Monitoring_ServerClosedEvent(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "ServerClosedEvent", class_MongoDB_Driver_Monitoring_ServerClosedEvent_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;

	return class_entry;
}
