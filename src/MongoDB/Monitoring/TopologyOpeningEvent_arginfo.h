/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: c72765c39e5f012ef819de515e42aa04bd19434b */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_TopologyOpeningEvent___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_TopologyOpeningEvent_getTopologyId, 0, 0, MongoDB\\BSON\\ObjectId, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_TopologyOpeningEvent___wakeup, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_Monitoring_TopologyOpeningEvent, __construct);
static ZEND_METHOD(MongoDB_Driver_Monitoring_TopologyOpeningEvent, getTopologyId);
static ZEND_METHOD(MongoDB_Driver_Monitoring_TopologyOpeningEvent, __wakeup);


static const zend_function_entry class_MongoDB_Driver_Monitoring_TopologyOpeningEvent_methods[] = {
	ZEND_ME(MongoDB_Driver_Monitoring_TopologyOpeningEvent, __construct, arginfo_class_MongoDB_Driver_Monitoring_TopologyOpeningEvent___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_TopologyOpeningEvent, getTopologyId, arginfo_class_MongoDB_Driver_Monitoring_TopologyOpeningEvent_getTopologyId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Monitoring_TopologyOpeningEvent, __wakeup, arginfo_class_MongoDB_Driver_Monitoring_TopologyOpeningEvent___wakeup, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Monitoring_TopologyOpeningEvent(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "TopologyOpeningEvent", class_MongoDB_Driver_Monitoring_TopologyOpeningEvent_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}
