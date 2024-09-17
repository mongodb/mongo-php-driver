/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 6ab1a9ad8b25b5d09ce12c1b0fa0e69885432d6f */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_TopologyOpeningEvent___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_TopologyOpeningEvent_getTopologyId, 0, 0, MongoDB\\BSON\\ObjectId, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_Monitoring_TopologyOpeningEvent, __construct);
static ZEND_METHOD(MongoDB_Driver_Monitoring_TopologyOpeningEvent, getTopologyId);


static const zend_function_entry class_MongoDB_Driver_Monitoring_TopologyOpeningEvent_methods[] = {
	ZEND_ME(MongoDB_Driver_Monitoring_TopologyOpeningEvent, __construct, arginfo_class_MongoDB_Driver_Monitoring_TopologyOpeningEvent___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_TopologyOpeningEvent, getTopologyId, arginfo_class_MongoDB_Driver_Monitoring_TopologyOpeningEvent_getTopologyId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Monitoring_TopologyOpeningEvent(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "TopologyOpeningEvent", class_MongoDB_Driver_Monitoring_TopologyOpeningEvent_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;

	return class_entry;
}
