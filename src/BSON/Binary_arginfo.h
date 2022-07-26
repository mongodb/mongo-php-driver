/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: f0ecc0b0ccfd6b0fceaa13cea21306a337022994 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Binary___construct, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Binary_getData, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Binary_getType, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Binary___set_state, 0, 1, MongoDB\\BSON\\Binary, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Binary___toString arginfo_class_MongoDB_BSON_Binary_getData

#define arginfo_class_MongoDB_BSON_Binary_serialize arginfo_class_MongoDB_BSON_Binary_getData

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Binary_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Binary___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Binary___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Binary_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Binary_jsonSerialize, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif


ZEND_METHOD(MongoDB_BSON_Binary, __construct);
ZEND_METHOD(MongoDB_BSON_Binary, getData);
ZEND_METHOD(MongoDB_BSON_Binary, getType);
ZEND_METHOD(MongoDB_BSON_Binary, __set_state);
ZEND_METHOD(MongoDB_BSON_Binary, __toString);
ZEND_METHOD(MongoDB_BSON_Binary, serialize);
ZEND_METHOD(MongoDB_BSON_Binary, unserialize);
ZEND_METHOD(MongoDB_BSON_Binary, __unserialize);
ZEND_METHOD(MongoDB_BSON_Binary, __serialize);
#if PHP_VERSION_ID >= 80000
ZEND_METHOD(MongoDB_BSON_Binary, jsonSerialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
ZEND_METHOD(MongoDB_BSON_Binary, jsonSerialize);
#endif


static const zend_function_entry class_MongoDB_BSON_Binary_methods[] = {
	ZEND_ME(MongoDB_BSON_Binary, __construct, arginfo_class_MongoDB_BSON_Binary___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Binary, getData, arginfo_class_MongoDB_BSON_Binary_getData, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Binary, getType, arginfo_class_MongoDB_BSON_Binary_getType, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Binary, __set_state, arginfo_class_MongoDB_BSON_Binary___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(MongoDB_BSON_Binary, __toString, arginfo_class_MongoDB_BSON_Binary___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Binary, serialize, arginfo_class_MongoDB_BSON_Binary_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Binary, unserialize, arginfo_class_MongoDB_BSON_Binary_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Binary, __unserialize, arginfo_class_MongoDB_BSON_Binary___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Binary, __serialize, arginfo_class_MongoDB_BSON_Binary___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Binary, jsonSerialize, arginfo_class_MongoDB_BSON_Binary_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Binary, jsonSerialize, arginfo_class_MongoDB_BSON_Binary_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_FE_END
};
