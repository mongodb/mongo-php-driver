/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 3207fb6b6a5b00471d08add24c41e062d7d2b43e */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandSubscriber_commandStarted, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\CommandStartedEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandSubscriber_commandSucceeded, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\CommandSucceededEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_CommandSubscriber_commandFailed, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\CommandFailedEvent, 0)
ZEND_END_ARG_INFO()


static const zend_function_entry class_MongoDB_Driver_Monitoring_CommandSubscriber_methods[] = {
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("commandStarted", NULL, arginfo_class_MongoDB_Driver_Monitoring_CommandSubscriber_commandStarted, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("commandStarted", NULL, arginfo_class_MongoDB_Driver_Monitoring_CommandSubscriber_commandStarted, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("commandSucceeded", NULL, arginfo_class_MongoDB_Driver_Monitoring_CommandSubscriber_commandSucceeded, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("commandSucceeded", NULL, arginfo_class_MongoDB_Driver_Monitoring_CommandSubscriber_commandSucceeded, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("commandFailed", NULL, arginfo_class_MongoDB_Driver_Monitoring_CommandSubscriber_commandFailed, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("commandFailed", NULL, arginfo_class_MongoDB_Driver_Monitoring_CommandSubscriber_commandFailed, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Monitoring_CommandSubscriber(zend_class_entry *class_entry_MongoDB_Driver_Monitoring_Subscriber)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "CommandSubscriber", class_MongoDB_Driver_Monitoring_CommandSubscriber_methods);
	class_entry = zend_register_internal_interface(&ce);
	zend_class_implements(class_entry, 1, class_entry_MongoDB_Driver_Monitoring_Subscriber);

	return class_entry;
}
