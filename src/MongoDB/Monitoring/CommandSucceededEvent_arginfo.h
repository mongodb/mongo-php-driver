/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: c369196418ec5b97da5b9780db7ca59d8a56e247 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getCommandName, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getDurationMicros, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getOperationId arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getCommandName

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getReply, 0, 0, IS_OBJECT, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getRequestId arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getCommandName

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getServer, 0, 0, MongoDB\\Driver\\Server, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getServiceId, 0, 0, MongoDB\\BSON\\ObjectId, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getServerConnectionId, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent___wakeup, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandSucceededEvent, __construct);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandSucceededEvent, getCommandName);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandSucceededEvent, getDurationMicros);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandSucceededEvent, getOperationId);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandSucceededEvent, getReply);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandSucceededEvent, getRequestId);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandSucceededEvent, getServer);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandSucceededEvent, getServiceId);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandSucceededEvent, getServerConnectionId);
static ZEND_METHOD(MongoDB_Driver_Monitoring_CommandSucceededEvent, __wakeup);


static const zend_function_entry class_MongoDB_Driver_Monitoring_CommandSucceededEvent_methods[] = {
	ZEND_ME(MongoDB_Driver_Monitoring_CommandSucceededEvent, __construct, arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandSucceededEvent, getCommandName, arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getCommandName, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandSucceededEvent, getDurationMicros, arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getDurationMicros, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandSucceededEvent, getOperationId, arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getOperationId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandSucceededEvent, getReply, arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getReply, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandSucceededEvent, getRequestId, arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getRequestId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandSucceededEvent, getServer, arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getServer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandSucceededEvent, getServiceId, arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getServiceId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandSucceededEvent, getServerConnectionId, arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent_getServerConnectionId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_CommandSucceededEvent, __wakeup, arginfo_class_MongoDB_Driver_Monitoring_CommandSucceededEvent___wakeup, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Monitoring_CommandSucceededEvent(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "CommandSucceededEvent", class_MongoDB_Driver_Monitoring_CommandSucceededEvent_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}
