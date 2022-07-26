/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 38571448536fc3d6796cf95bcff83bfcd2d48068 */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_MinKey___set_state, 0, 1, MongoDB\\BSON\\MinKey, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_MinKey_serialize, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_MinKey_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_MinKey___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_MinKey___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_MinKey_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_MinKey_jsonSerialize, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif


ZEND_METHOD(MongoDB_BSON_MinKey, __set_state);
ZEND_METHOD(MongoDB_BSON_MinKey, serialize);
ZEND_METHOD(MongoDB_BSON_MinKey, unserialize);
ZEND_METHOD(MongoDB_BSON_MinKey, __unserialize);
ZEND_METHOD(MongoDB_BSON_MinKey, __serialize);
#if PHP_VERSION_ID >= 80000
ZEND_METHOD(MongoDB_BSON_MinKey, jsonSerialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
ZEND_METHOD(MongoDB_BSON_MinKey, jsonSerialize);
#endif


static const zend_function_entry class_MongoDB_BSON_MinKey_methods[] = {
	ZEND_ME(MongoDB_BSON_MinKey, __set_state, arginfo_class_MongoDB_BSON_MinKey___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(MongoDB_BSON_MinKey, serialize, arginfo_class_MongoDB_BSON_MinKey_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_MinKey, unserialize, arginfo_class_MongoDB_BSON_MinKey_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_MinKey, __unserialize, arginfo_class_MongoDB_BSON_MinKey___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_MinKey, __serialize, arginfo_class_MongoDB_BSON_MinKey___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_MinKey, jsonSerialize, arginfo_class_MongoDB_BSON_MinKey_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_MinKey, jsonSerialize, arginfo_class_MongoDB_BSON_MinKey_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_FE_END
};
