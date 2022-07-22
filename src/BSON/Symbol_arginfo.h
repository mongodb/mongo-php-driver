/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: ecfb2e9f3329d6ca822bfd8d4720b08543dadafd */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Symbol___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Symbol___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Symbol_serialize arginfo_class_MongoDB_BSON_Symbol___toString

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Symbol_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Symbol___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Symbol___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Symbol_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Symbol_jsonSerialize, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif


ZEND_METHOD(MongoDB_BSON_Symbol, __construct);
ZEND_METHOD(MongoDB_BSON_Symbol, __toString);
ZEND_METHOD(MongoDB_BSON_Symbol, serialize);
ZEND_METHOD(MongoDB_BSON_Symbol, unserialize);
ZEND_METHOD(MongoDB_BSON_Symbol, __unserialize);
ZEND_METHOD(MongoDB_BSON_Symbol, __serialize);
#if PHP_VERSION_ID >= 80000
ZEND_METHOD(MongoDB_BSON_Symbol, jsonSerialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
ZEND_METHOD(MongoDB_BSON_Symbol, jsonSerialize);
#endif


static const zend_function_entry class_MongoDB_BSON_Symbol_methods[] = {
	ZEND_ME(MongoDB_BSON_Symbol, __construct, arginfo_class_MongoDB_BSON_Symbol___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Symbol, __toString, arginfo_class_MongoDB_BSON_Symbol___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Symbol, serialize, arginfo_class_MongoDB_BSON_Symbol_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Symbol, unserialize, arginfo_class_MongoDB_BSON_Symbol_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Symbol, __unserialize, arginfo_class_MongoDB_BSON_Symbol___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Symbol, __serialize, arginfo_class_MongoDB_BSON_Symbol___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Symbol, jsonSerialize, arginfo_class_MongoDB_BSON_Symbol_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Symbol, jsonSerialize, arginfo_class_MongoDB_BSON_Symbol_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_FE_END
};
