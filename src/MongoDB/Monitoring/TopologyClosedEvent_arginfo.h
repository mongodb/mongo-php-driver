/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 3ab90b4e8bbb8fe5d0b2248e40b102ab029ac6f1 */

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
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);
#else
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;
#endif

	zval property_topologyId_default_value;
	ZVAL_UNDEF(&property_topologyId_default_value);
	zend_string *property_topologyId_name = zend_string_init("topologyId", sizeof("topologyId") - 1, 1);
	zend_string *property_topologyId_class_MongoDB_BSON_ObjectId = zend_string_init("MongoDB\\BSON\\ObjectId", sizeof("MongoDB\\BSON\\ObjectId")-1, 1);
	zend_declare_typed_property(class_entry, property_topologyId_name, &property_topologyId_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_topologyId_class_MongoDB_BSON_ObjectId, 0, 0));
	zend_string_release(property_topologyId_name);

	return class_entry;
}
