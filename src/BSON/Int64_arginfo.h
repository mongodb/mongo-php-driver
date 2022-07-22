/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 23a49399030aec0b31a2a2136032866c40962842 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Int64___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Int64___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Int64_serialize arginfo_class_MongoDB_BSON_Int64___toString

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Int64_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Int64___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Int64___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Int64_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Int64_jsonSerialize, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif


ZEND_METHOD(MongoDB_BSON_Int64, __construct);
ZEND_METHOD(MongoDB_BSON_Int64, __toString);
ZEND_METHOD(MongoDB_BSON_Int64, serialize);
ZEND_METHOD(MongoDB_BSON_Int64, unserialize);
ZEND_METHOD(MongoDB_BSON_Int64, __unserialize);
ZEND_METHOD(MongoDB_BSON_Int64, __serialize);
#if PHP_VERSION_ID >= 80000
ZEND_METHOD(MongoDB_BSON_Int64, jsonSerialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
ZEND_METHOD(MongoDB_BSON_Int64, jsonSerialize);
#endif


static const zend_function_entry class_MongoDB_BSON_Int64_methods[] = {
	ZEND_ME(MongoDB_BSON_Int64, __construct, arginfo_class_MongoDB_BSON_Int64___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Int64, __toString, arginfo_class_MongoDB_BSON_Int64___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Int64, serialize, arginfo_class_MongoDB_BSON_Int64_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Int64, unserialize, arginfo_class_MongoDB_BSON_Int64_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Int64, __unserialize, arginfo_class_MongoDB_BSON_Int64___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Int64, __serialize, arginfo_class_MongoDB_BSON_Int64___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Int64, jsonSerialize, arginfo_class_MongoDB_BSON_Int64_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Int64, jsonSerialize, arginfo_class_MongoDB_BSON_Int64_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_FE_END
};
