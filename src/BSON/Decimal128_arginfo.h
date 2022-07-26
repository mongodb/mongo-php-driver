/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 27c67c871c2b56ef3091d5ae67f83ef4c6b050ad */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, value, IS_STRING, 0, "\'\'")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128___set_state, 0, 1, MongoDB\\BSON\\Decimal128, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Decimal128_serialize arginfo_class_MongoDB_BSON_Decimal128___toString

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128_jsonSerialize, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif


ZEND_METHOD(MongoDB_BSON_Decimal128, __construct);
ZEND_METHOD(MongoDB_BSON_Decimal128, __toString);
ZEND_METHOD(MongoDB_BSON_Decimal128, __set_state);
ZEND_METHOD(MongoDB_BSON_Decimal128, serialize);
ZEND_METHOD(MongoDB_BSON_Decimal128, unserialize);
ZEND_METHOD(MongoDB_BSON_Decimal128, __unserialize);
ZEND_METHOD(MongoDB_BSON_Decimal128, __serialize);
#if PHP_VERSION_ID >= 80000
ZEND_METHOD(MongoDB_BSON_Decimal128, jsonSerialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
ZEND_METHOD(MongoDB_BSON_Decimal128, jsonSerialize);
#endif


static const zend_function_entry class_MongoDB_BSON_Decimal128_methods[] = {
	ZEND_ME(MongoDB_BSON_Decimal128, __construct, arginfo_class_MongoDB_BSON_Decimal128___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Decimal128, __toString, arginfo_class_MongoDB_BSON_Decimal128___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Decimal128, __set_state, arginfo_class_MongoDB_BSON_Decimal128___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(MongoDB_BSON_Decimal128, serialize, arginfo_class_MongoDB_BSON_Decimal128_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Decimal128, unserialize, arginfo_class_MongoDB_BSON_Decimal128_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Decimal128, __unserialize, arginfo_class_MongoDB_BSON_Decimal128___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Decimal128, __serialize, arginfo_class_MongoDB_BSON_Decimal128___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Decimal128, jsonSerialize, arginfo_class_MongoDB_BSON_Decimal128_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Decimal128, jsonSerialize, arginfo_class_MongoDB_BSON_Decimal128_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_FE_END
};
