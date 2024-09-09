/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: c636ee276d6ebc18186a6ef4b8f67c48f5b4c29c */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_TopologyClosedEvent___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_TopologyClosedEvent_getTopologyId, 0, 0, MongoDB\\BSON\\ObjectId, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_Monitoring_TopologyClosedEvent, __construct);
static ZEND_METHOD(MongoDB_Driver_Monitoring_TopologyClosedEvent, getTopologyId);


static const zend_function_entry class_MongoDB_Driver_Monitoring_TopologyClosedEvent_methods[] = {
	ZEND_ME(MongoDB_Driver_Monitoring_TopologyClosedEvent, __construct, arginfo_class_MongoDB_Driver_Monitoring_TopologyClosedEvent___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_TopologyClosedEvent, getTopologyId, arginfo_class_MongoDB_Driver_Monitoring_TopologyClosedEvent_getTopologyId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Monitoring_TopologyClosedEvent(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "TopologyClosedEvent", class_MongoDB_Driver_Monitoring_TopologyClosedEvent_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;

	return class_entry;
}
