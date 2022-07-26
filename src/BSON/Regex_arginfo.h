/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 76a05a27305d859539e9bc82818e7b37aeddeabf */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Regex___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, flags, IS_STRING, 0, "\'\'")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Regex_getPattern, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Regex_getFlags arginfo_class_MongoDB_BSON_Regex_getPattern

#define arginfo_class_MongoDB_BSON_Regex___toString arginfo_class_MongoDB_BSON_Regex_getPattern

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Regex___set_state, 0, 1, MongoDB\\BSON\\Regex, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Regex_serialize arginfo_class_MongoDB_BSON_Regex_getPattern

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Regex_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Regex___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Regex___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Regex_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Regex_jsonSerialize, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif


ZEND_METHOD(MongoDB_BSON_Regex, __construct);
ZEND_METHOD(MongoDB_BSON_Regex, getPattern);
ZEND_METHOD(MongoDB_BSON_Regex, getFlags);
ZEND_METHOD(MongoDB_BSON_Regex, __toString);
ZEND_METHOD(MongoDB_BSON_Regex, __set_state);
ZEND_METHOD(MongoDB_BSON_Regex, serialize);
ZEND_METHOD(MongoDB_BSON_Regex, unserialize);
ZEND_METHOD(MongoDB_BSON_Regex, __unserialize);
ZEND_METHOD(MongoDB_BSON_Regex, __serialize);
#if PHP_VERSION_ID >= 80000
ZEND_METHOD(MongoDB_BSON_Regex, jsonSerialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
ZEND_METHOD(MongoDB_BSON_Regex, jsonSerialize);
#endif


static const zend_function_entry class_MongoDB_BSON_Regex_methods[] = {
	ZEND_ME(MongoDB_BSON_Regex, __construct, arginfo_class_MongoDB_BSON_Regex___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Regex, getPattern, arginfo_class_MongoDB_BSON_Regex_getPattern, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Regex, getFlags, arginfo_class_MongoDB_BSON_Regex_getFlags, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Regex, __toString, arginfo_class_MongoDB_BSON_Regex___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Regex, __set_state, arginfo_class_MongoDB_BSON_Regex___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(MongoDB_BSON_Regex, serialize, arginfo_class_MongoDB_BSON_Regex_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Regex, unserialize, arginfo_class_MongoDB_BSON_Regex_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Regex, __unserialize, arginfo_class_MongoDB_BSON_Regex___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Regex, __serialize, arginfo_class_MongoDB_BSON_Regex___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Regex, jsonSerialize, arginfo_class_MongoDB_BSON_Regex_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Regex, jsonSerialize, arginfo_class_MongoDB_BSON_Regex_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_FE_END
};
