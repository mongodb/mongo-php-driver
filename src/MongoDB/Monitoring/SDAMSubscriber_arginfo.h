/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: b7d58477d2ac5f63bdfb04801eb6517aff5dd8df */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverChanged, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\ServerChangedEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverClosed, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\ServerClosedEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverOpening, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\ServerOpeningEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverHeartbeatFailed, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\ServerHeartbeatFailedEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverHeartbeatStarted, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\ServerHeartbeatStartedEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverHeartbeatSucceeded, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\ServerHeartbeatSucceededEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_topologyChanged, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\TopologyChangedEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_topologyClosed, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\TopologyClosedEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_topologyOpening, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\TopologyOpeningEvent, 0)
ZEND_END_ARG_INFO()


static const zend_function_entry class_MongoDB_Driver_Monitoring_SDAMSubscriber_methods[] = {
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("serverChanged", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverChanged, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("serverChanged", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverChanged, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("serverClosed", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverClosed, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("serverClosed", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverClosed, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("serverOpening", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverOpening, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("serverOpening", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverOpening, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("serverHeartbeatFailed", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverHeartbeatFailed, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("serverHeartbeatFailed", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverHeartbeatFailed, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("serverHeartbeatStarted", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverHeartbeatStarted, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("serverHeartbeatStarted", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverHeartbeatStarted, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("serverHeartbeatSucceeded", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverHeartbeatSucceeded, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("serverHeartbeatSucceeded", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_serverHeartbeatSucceeded, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("topologyChanged", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_topologyChanged, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("topologyChanged", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_topologyChanged, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("topologyClosed", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_topologyClosed, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("topologyClosed", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_topologyClosed, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("topologyOpening", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_topologyOpening, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("topologyOpening", NULL, arginfo_class_MongoDB_Driver_Monitoring_SDAMSubscriber_topologyOpening, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Monitoring_SDAMSubscriber(zend_class_entry *class_entry_MongoDB_Driver_Monitoring_Subscriber)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "SDAMSubscriber", class_MongoDB_Driver_Monitoring_SDAMSubscriber_methods);
	class_entry = zend_register_internal_interface(&ce);
	zend_class_implements(class_entry, 1, class_entry_MongoDB_Driver_Monitoring_Subscriber);

	return class_entry;
}
