/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 4d9c8b8131126ce2e67fe1659205aaf7eee8c14e */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_MongoDB_BSON_fromJSON, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, json, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_MongoDB_BSON_fromPHP, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_MASK(0, value, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_MongoDB_BSON_toCanonicalExtendedJSON, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, bson, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_MongoDB_BSON_toJSON arginfo_MongoDB_BSON_toCanonicalExtendedJSON

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_MongoDB_BSON_toPHP, 0, 1, MAY_BE_ARRAY|MAY_BE_OBJECT)
	ZEND_ARG_TYPE_INFO(0, bson, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, typemap, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

#define arginfo_MongoDB_BSON_toRelaxedExtendedJSON arginfo_MongoDB_BSON_toCanonicalExtendedJSON

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_MongoDB_Driver_Monitoring_addSubscriber, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, subscriber, MongoDB\\Driver\\Monitoring\\Subscriber, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_MongoDB_Driver_Monitoring_mongoc_log, 0, 3, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, level, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, domain, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_MongoDB_Driver_Monitoring_removeSubscriber arginfo_MongoDB_Driver_Monitoring_addSubscriber


ZEND_FUNCTION(MongoDB_BSON_fromJSON);
ZEND_FUNCTION(MongoDB_BSON_fromPHP);
ZEND_FUNCTION(MongoDB_BSON_toCanonicalExtendedJSON);
ZEND_FUNCTION(MongoDB_BSON_toJSON);
ZEND_FUNCTION(MongoDB_BSON_toPHP);
ZEND_FUNCTION(MongoDB_BSON_toRelaxedExtendedJSON);
ZEND_FUNCTION(MongoDB_Driver_Monitoring_addSubscriber);
ZEND_FUNCTION(MongoDB_Driver_Monitoring_mongoc_log);
ZEND_FUNCTION(MongoDB_Driver_Monitoring_removeSubscriber);


static const zend_function_entry ext_functions[] = {
	ZEND_NS_DEP_FALIAS("MongoDB\\BSON", fromJSON, MongoDB_BSON_fromJSON, arginfo_MongoDB_BSON_fromJSON)
	ZEND_NS_DEP_FALIAS("MongoDB\\BSON", fromPHP, MongoDB_BSON_fromPHP, arginfo_MongoDB_BSON_fromPHP)
	ZEND_NS_DEP_FALIAS("MongoDB\\BSON", toCanonicalExtendedJSON, MongoDB_BSON_toCanonicalExtendedJSON, arginfo_MongoDB_BSON_toCanonicalExtendedJSON)
	ZEND_NS_DEP_FALIAS("MongoDB\\BSON", toJSON, MongoDB_BSON_toJSON, arginfo_MongoDB_BSON_toJSON)
	ZEND_NS_DEP_FALIAS("MongoDB\\BSON", toPHP, MongoDB_BSON_toPHP, arginfo_MongoDB_BSON_toPHP)
	ZEND_NS_DEP_FALIAS("MongoDB\\BSON", toRelaxedExtendedJSON, MongoDB_BSON_toRelaxedExtendedJSON, arginfo_MongoDB_BSON_toRelaxedExtendedJSON)
	ZEND_NS_FALIAS("MongoDB\\Driver\\Monitoring", addSubscriber, MongoDB_Driver_Monitoring_addSubscriber, arginfo_MongoDB_Driver_Monitoring_addSubscriber)
	ZEND_NS_FALIAS("MongoDB\\Driver\\Monitoring", mongoc_log, MongoDB_Driver_Monitoring_mongoc_log, arginfo_MongoDB_Driver_Monitoring_mongoc_log)
	ZEND_NS_FALIAS("MongoDB\\Driver\\Monitoring", removeSubscriber, MongoDB_Driver_Monitoring_removeSubscriber, arginfo_MongoDB_Driver_Monitoring_removeSubscriber)
	ZEND_FE_END
};
