/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: c566aa463b811fdc8f93a620d66ee19271fc12ec */

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp___construct, 0, 0, 2)
	ZEND_ARG_TYPE_MASK(0, increment, MAY_BE_LONG|MAY_BE_STRING, NULL)
	ZEND_ARG_TYPE_MASK(0, timestamp, MAY_BE_LONG|MAY_BE_STRING, NULL)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, increment)
	ZEND_ARG_INFO(0, timestamp)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp_getTimestamp, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Timestamp_getIncrement arginfo_class_MongoDB_BSON_Timestamp_getTimestamp

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp___set_state, 0, 1, IS_OBJECT, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Timestamp_serialize arginfo_class_MongoDB_BSON_Timestamp___toString

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp_jsonSerialize, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif


#if PHP_VERSION_ID >= 80000
ZEND_METHOD(MongoDB_BSON_Timestamp, __construct);
#endif
#if !(PHP_VERSION_ID >= 80000)
ZEND_METHOD(MongoDB_BSON_Timestamp, __construct);
#endif
ZEND_METHOD(MongoDB_BSON_Timestamp, getTimestamp);
ZEND_METHOD(MongoDB_BSON_Timestamp, getIncrement);
ZEND_METHOD(MongoDB_BSON_Timestamp, __toString);
ZEND_METHOD(MongoDB_BSON_Timestamp, __set_state);
ZEND_METHOD(MongoDB_BSON_Timestamp, serialize);
ZEND_METHOD(MongoDB_BSON_Timestamp, unserialize);
ZEND_METHOD(MongoDB_BSON_Timestamp, __unserialize);
ZEND_METHOD(MongoDB_BSON_Timestamp, __serialize);
#if PHP_VERSION_ID >= 80000
ZEND_METHOD(MongoDB_BSON_Timestamp, jsonSerialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
ZEND_METHOD(MongoDB_BSON_Timestamp, jsonSerialize);
#endif


static const zend_function_entry class_MongoDB_BSON_Timestamp_methods[] = {
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Timestamp, __construct, arginfo_class_MongoDB_BSON_Timestamp___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Timestamp, __construct, arginfo_class_MongoDB_BSON_Timestamp___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_Timestamp, getTimestamp, arginfo_class_MongoDB_BSON_Timestamp_getTimestamp, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Timestamp, getIncrement, arginfo_class_MongoDB_BSON_Timestamp_getIncrement, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Timestamp, __toString, arginfo_class_MongoDB_BSON_Timestamp___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Timestamp, __set_state, arginfo_class_MongoDB_BSON_Timestamp___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(MongoDB_BSON_Timestamp, serialize, arginfo_class_MongoDB_BSON_Timestamp_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Timestamp, unserialize, arginfo_class_MongoDB_BSON_Timestamp_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Timestamp, __unserialize, arginfo_class_MongoDB_BSON_Timestamp___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Timestamp, __serialize, arginfo_class_MongoDB_BSON_Timestamp___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Timestamp, jsonSerialize, arginfo_class_MongoDB_BSON_Timestamp_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Timestamp, jsonSerialize, arginfo_class_MongoDB_BSON_Timestamp_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_FE_END
};
