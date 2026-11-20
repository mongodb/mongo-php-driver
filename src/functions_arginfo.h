/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 49f42083b44ce54a5dce3d4f619b0d7210437527 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_MongoDB_Driver_Monitoring_addSubscriber, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, subscriber, MongoDB\\Driver\\Monitoring\\Subscriber, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_MongoDB_Driver_Monitoring_mongoc_log, 0, 3, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, level, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, domain, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_MongoDB_Driver_Monitoring_removeSubscriber arginfo_MongoDB_Driver_Monitoring_addSubscriber

ZEND_FUNCTION(MongoDB_Driver_Monitoring_addSubscriber);
ZEND_FUNCTION(MongoDB_Driver_Monitoring_mongoc_log);
ZEND_FUNCTION(MongoDB_Driver_Monitoring_removeSubscriber);

static const zend_function_entry ext_functions[] = {
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY(ZEND_NS_NAME("MongoDB\\Driver\\Monitoring", "addSubscriber"), zif_MongoDB_Driver_Monitoring_addSubscriber, arginfo_MongoDB_Driver_Monitoring_addSubscriber, 0, NULL, NULL)
#else
	ZEND_RAW_FENTRY(ZEND_NS_NAME("MongoDB\\Driver\\Monitoring", "addSubscriber"), zif_MongoDB_Driver_Monitoring_addSubscriber, arginfo_MongoDB_Driver_Monitoring_addSubscriber, 0)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY(ZEND_NS_NAME("MongoDB\\Driver\\Monitoring", "mongoc_log"), zif_MongoDB_Driver_Monitoring_mongoc_log, arginfo_MongoDB_Driver_Monitoring_mongoc_log, 0, NULL, NULL)
#else
	ZEND_RAW_FENTRY(ZEND_NS_NAME("MongoDB\\Driver\\Monitoring", "mongoc_log"), zif_MongoDB_Driver_Monitoring_mongoc_log, arginfo_MongoDB_Driver_Monitoring_mongoc_log, 0)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY(ZEND_NS_NAME("MongoDB\\Driver\\Monitoring", "removeSubscriber"), zif_MongoDB_Driver_Monitoring_removeSubscriber, arginfo_MongoDB_Driver_Monitoring_removeSubscriber, 0, NULL, NULL)
#else
	ZEND_RAW_FENTRY(ZEND_NS_NAME("MongoDB\\Driver\\Monitoring", "removeSubscriber"), zif_MongoDB_Driver_Monitoring_removeSubscriber, arginfo_MongoDB_Driver_Monitoring_removeSubscriber, 0)
#endif
	ZEND_FE_END
};
