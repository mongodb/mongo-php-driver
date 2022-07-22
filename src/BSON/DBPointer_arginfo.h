/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: cb2b4641c0a3de9a938b4515667a976e9f21ec50 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_DBPointer___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_DBPointer___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_DBPointer_serialize arginfo_class_MongoDB_BSON_DBPointer___toString

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_DBPointer_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_DBPointer___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_DBPointer___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_DBPointer_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_DBPointer_jsonSerialize, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif


ZEND_METHOD(MongoDB_BSON_DBPointer, __construct);
ZEND_METHOD(MongoDB_BSON_DBPointer, __toString);
ZEND_METHOD(MongoDB_BSON_DBPointer, serialize);
ZEND_METHOD(MongoDB_BSON_DBPointer, unserialize);
ZEND_METHOD(MongoDB_BSON_DBPointer, __unserialize);
ZEND_METHOD(MongoDB_BSON_DBPointer, __serialize);
#if PHP_VERSION_ID >= 80000
ZEND_METHOD(MongoDB_BSON_DBPointer, jsonSerialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
ZEND_METHOD(MongoDB_BSON_DBPointer, jsonSerialize);
#endif


static const zend_function_entry class_MongoDB_BSON_DBPointer_methods[] = {
	ZEND_ME(MongoDB_BSON_DBPointer, __construct, arginfo_class_MongoDB_BSON_DBPointer___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_DBPointer, __toString, arginfo_class_MongoDB_BSON_DBPointer___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_DBPointer, serialize, arginfo_class_MongoDB_BSON_DBPointer_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_DBPointer, unserialize, arginfo_class_MongoDB_BSON_DBPointer_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_DBPointer, __unserialize, arginfo_class_MongoDB_BSON_DBPointer___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_DBPointer, __serialize, arginfo_class_MongoDB_BSON_DBPointer___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_DBPointer, jsonSerialize, arginfo_class_MongoDB_BSON_DBPointer_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_DBPointer, jsonSerialize, arginfo_class_MongoDB_BSON_DBPointer_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_FE_END
};
