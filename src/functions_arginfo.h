/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 964efb254b4bb8c11a46e51a3005507aabd8d726 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_MongoDB_BSON_fromJSON, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, json, IS_STRING, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_MongoDB_BSON_fromPHP, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_MASK(0, value, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_MongoDB_BSON_fromPHP, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_MongoDB_BSON_toCanonicalExtendedJSON, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, bson, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_MongoDB_BSON_toJSON arginfo_MongoDB_BSON_toCanonicalExtendedJSON

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_MongoDB_BSON_toPHP, 0, 1, MAY_BE_ARRAY|MAY_BE_OBJECT)
	ZEND_ARG_TYPE_INFO(0, bson, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, typemap, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_MongoDB_BSON_toPHP, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, bson, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, typemap, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()
#endif

#define arginfo_MongoDB_BSON_toRelaxedExtendedJSON arginfo_MongoDB_BSON_toCanonicalExtendedJSON

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_MongoDB_Driver_Monitoring_addSubscriber, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, subscriber, MongoDB\\Driver\\Monitoring\\Subscriber, 0)
ZEND_END_ARG_INFO()

#define arginfo_MongoDB_Driver_Monitoring_removeSubscriber arginfo_MongoDB_Driver_Monitoring_addSubscriber


ZEND_FUNCTION(fromJSON);
#if PHP_VERSION_ID >= 80000
ZEND_FUNCTION(fromPHP);
#endif
#if !(PHP_VERSION_ID >= 80000)
ZEND_FUNCTION(fromPHP);
#endif
ZEND_FUNCTION(toCanonicalExtendedJSON);
ZEND_FUNCTION(toJSON);
#if PHP_VERSION_ID >= 80000
ZEND_FUNCTION(toPHP);
#endif
#if !(PHP_VERSION_ID >= 80000)
ZEND_FUNCTION(toPHP);
#endif
ZEND_FUNCTION(toRelaxedExtendedJSON);
ZEND_FUNCTION(addSubscriber);
ZEND_FUNCTION(removeSubscriber);


static const zend_function_entry ext_functions[] = {
	ZEND_NS_FE("MongoDB\\BSON", fromJSON, arginfo_MongoDB_BSON_fromJSON)
#if PHP_VERSION_ID >= 80000
	ZEND_NS_FE("MongoDB\\BSON", fromPHP, arginfo_MongoDB_BSON_fromPHP)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_NS_FE("MongoDB\\BSON", fromPHP, arginfo_MongoDB_BSON_fromPHP)
#endif
	ZEND_NS_FE("MongoDB\\BSON", toCanonicalExtendedJSON, arginfo_MongoDB_BSON_toCanonicalExtendedJSON)
	ZEND_NS_FE("MongoDB\\BSON", toJSON, arginfo_MongoDB_BSON_toJSON)
#if PHP_VERSION_ID >= 80000
	ZEND_NS_FE("MongoDB\\BSON", toPHP, arginfo_MongoDB_BSON_toPHP)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_NS_FE("MongoDB\\BSON", toPHP, arginfo_MongoDB_BSON_toPHP)
#endif
	ZEND_NS_FE("MongoDB\\BSON", toRelaxedExtendedJSON, arginfo_MongoDB_BSON_toRelaxedExtendedJSON)
	ZEND_NS_FE("MongoDB\\Driver\\Monitoring", addSubscriber, arginfo_MongoDB_Driver_Monitoring_addSubscriber)
	ZEND_NS_FE("MongoDB\\Driver\\Monitoring", removeSubscriber, arginfo_MongoDB_Driver_Monitoring_removeSubscriber)
	ZEND_FE_END
};
