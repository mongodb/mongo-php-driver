/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 78669572ebbe92cabbefacd145fca8332bc7cb75 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Undefined___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Undefined___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Undefined_serialize arginfo_class_MongoDB_BSON_Undefined___toString

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Undefined_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Undefined___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Undefined___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Undefined_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Undefined_jsonSerialize, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif


ZEND_METHOD(MongoDB_BSON_Undefined, __construct);
ZEND_METHOD(MongoDB_BSON_Undefined, __toString);
ZEND_METHOD(MongoDB_BSON_Undefined, serialize);
ZEND_METHOD(MongoDB_BSON_Undefined, unserialize);
ZEND_METHOD(MongoDB_BSON_Undefined, __unserialize);
ZEND_METHOD(MongoDB_BSON_Undefined, __serialize);
#if PHP_VERSION_ID >= 80000
ZEND_METHOD(MongoDB_BSON_Undefined, jsonSerialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
ZEND_METHOD(MongoDB_BSON_Undefined, jsonSerialize);
#endif


static const zend_function_entry class_MongoDB_BSON_Undefined_methods[] = {
	ZEND_ME(MongoDB_BSON_Undefined, __construct, arginfo_class_MongoDB_BSON_Undefined___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Undefined, __toString, arginfo_class_MongoDB_BSON_Undefined___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Undefined, serialize, arginfo_class_MongoDB_BSON_Undefined_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Undefined, unserialize, arginfo_class_MongoDB_BSON_Undefined_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Undefined, __unserialize, arginfo_class_MongoDB_BSON_Undefined___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Undefined, __serialize, arginfo_class_MongoDB_BSON_Undefined___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Undefined, jsonSerialize, arginfo_class_MongoDB_BSON_Undefined_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Undefined, jsonSerialize, arginfo_class_MongoDB_BSON_Undefined_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_FE_END
};
