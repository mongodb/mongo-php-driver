/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 4920ad21bd26ea586d4322d49bf44c9c3530e494 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Monitoring_LogSubscriber_log, 0, 3, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, level, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, domain, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
ZEND_END_ARG_INFO()




static const zend_function_entry class_MongoDB_Driver_Monitoring_LogSubscriber_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_Driver_Monitoring_LogSubscriber, log, arginfo_class_MongoDB_Driver_Monitoring_LogSubscriber_log, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Monitoring_LogSubscriber(zend_class_entry *class_entry_MongoDB_Driver_Monitoring_Subscriber)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "LogSubscriber", class_MongoDB_Driver_Monitoring_LogSubscriber_methods);
	class_entry = zend_register_internal_interface(&ce);
	zend_class_implements(class_entry, 1, class_entry_MongoDB_Driver_Monitoring_Subscriber);

	zval const_LEVEL_ERROR_value;
	ZVAL_LONG(&const_LEVEL_ERROR_value, MONGOC_LOG_LEVEL_ERROR);
	zend_string *const_LEVEL_ERROR_name = zend_string_init_interned("LEVEL_ERROR", sizeof("LEVEL_ERROR") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_LEVEL_ERROR_name, &const_LEVEL_ERROR_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_LEVEL_ERROR_name);
	ZEND_ASSERT(MONGOC_LOG_LEVEL_ERROR == 0);

	zval const_LEVEL_CRITICAL_value;
	ZVAL_LONG(&const_LEVEL_CRITICAL_value, MONGOC_LOG_LEVEL_CRITICAL);
	zend_string *const_LEVEL_CRITICAL_name = zend_string_init_interned("LEVEL_CRITICAL", sizeof("LEVEL_CRITICAL") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_LEVEL_CRITICAL_name, &const_LEVEL_CRITICAL_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_LEVEL_CRITICAL_name);
	ZEND_ASSERT(MONGOC_LOG_LEVEL_CRITICAL == 1);

	zval const_LEVEL_WARNING_value;
	ZVAL_LONG(&const_LEVEL_WARNING_value, MONGOC_LOG_LEVEL_WARNING);
	zend_string *const_LEVEL_WARNING_name = zend_string_init_interned("LEVEL_WARNING", sizeof("LEVEL_WARNING") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_LEVEL_WARNING_name, &const_LEVEL_WARNING_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_LEVEL_WARNING_name);
	ZEND_ASSERT(MONGOC_LOG_LEVEL_WARNING == 2);

	zval const_LEVEL_MESSAGE_value;
	ZVAL_LONG(&const_LEVEL_MESSAGE_value, MONGOC_LOG_LEVEL_MESSAGE);
	zend_string *const_LEVEL_MESSAGE_name = zend_string_init_interned("LEVEL_MESSAGE", sizeof("LEVEL_MESSAGE") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_LEVEL_MESSAGE_name, &const_LEVEL_MESSAGE_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_LEVEL_MESSAGE_name);
	ZEND_ASSERT(MONGOC_LOG_LEVEL_MESSAGE == 3);

	zval const_LEVEL_INFO_value;
	ZVAL_LONG(&const_LEVEL_INFO_value, MONGOC_LOG_LEVEL_INFO);
	zend_string *const_LEVEL_INFO_name = zend_string_init_interned("LEVEL_INFO", sizeof("LEVEL_INFO") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_LEVEL_INFO_name, &const_LEVEL_INFO_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_LEVEL_INFO_name);
	ZEND_ASSERT(MONGOC_LOG_LEVEL_INFO == 4);

	zval const_LEVEL_DEBUG_value;
	ZVAL_LONG(&const_LEVEL_DEBUG_value, MONGOC_LOG_LEVEL_DEBUG);
	zend_string *const_LEVEL_DEBUG_name = zend_string_init_interned("LEVEL_DEBUG", sizeof("LEVEL_DEBUG") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_LEVEL_DEBUG_name, &const_LEVEL_DEBUG_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_LEVEL_DEBUG_name);
	ZEND_ASSERT(MONGOC_LOG_LEVEL_DEBUG == 5);

	return class_entry;
}
