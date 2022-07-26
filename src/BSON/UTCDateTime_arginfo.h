/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 7c0beb464e92cd5fc7bf6f4d0bea48dabb7875db */

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___construct, 0, 0, 0)
	ZEND_ARG_OBJ_TYPE_MASK(0, milliseconds, DateTime|DateTimeImmutable, MAY_BE_LONG|MAY_BE_STRING|MAY_BE_DOUBLE|MAY_BE_NULL, "null")
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___construct, 0, 0, 0)
	ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, milliseconds, "null")
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime_toDateTime, 0, 0, DateTime, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___set_state, 0, 1, MongoDB\\BSON\\UTCDateTime, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_UTCDateTime_serialize arginfo_class_MongoDB_BSON_UTCDateTime___toString

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime_jsonSerialize, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif


#if PHP_VERSION_ID >= 80000
ZEND_METHOD(MongoDB_BSON_UTCDateTime, __construct);
#endif
#if !(PHP_VERSION_ID >= 80000)
ZEND_METHOD(MongoDB_BSON_UTCDateTime, __construct);
#endif
ZEND_METHOD(MongoDB_BSON_UTCDateTime, toDateTime);
ZEND_METHOD(MongoDB_BSON_UTCDateTime, __toString);
ZEND_METHOD(MongoDB_BSON_UTCDateTime, __set_state);
ZEND_METHOD(MongoDB_BSON_UTCDateTime, serialize);
ZEND_METHOD(MongoDB_BSON_UTCDateTime, unserialize);
ZEND_METHOD(MongoDB_BSON_UTCDateTime, __unserialize);
ZEND_METHOD(MongoDB_BSON_UTCDateTime, __serialize);
#if PHP_VERSION_ID >= 80000
ZEND_METHOD(MongoDB_BSON_UTCDateTime, jsonSerialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
ZEND_METHOD(MongoDB_BSON_UTCDateTime, jsonSerialize);
#endif


static const zend_function_entry class_MongoDB_BSON_UTCDateTime_methods[] = {
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_UTCDateTime, __construct, arginfo_class_MongoDB_BSON_UTCDateTime___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_UTCDateTime, __construct, arginfo_class_MongoDB_BSON_UTCDateTime___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_UTCDateTime, toDateTime, arginfo_class_MongoDB_BSON_UTCDateTime_toDateTime, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, __toString, arginfo_class_MongoDB_BSON_UTCDateTime___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, __set_state, arginfo_class_MongoDB_BSON_UTCDateTime___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(MongoDB_BSON_UTCDateTime, serialize, arginfo_class_MongoDB_BSON_UTCDateTime_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, unserialize, arginfo_class_MongoDB_BSON_UTCDateTime_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, __unserialize, arginfo_class_MongoDB_BSON_UTCDateTime___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, __serialize, arginfo_class_MongoDB_BSON_UTCDateTime___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_UTCDateTime, jsonSerialize, arginfo_class_MongoDB_BSON_UTCDateTime_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_UTCDateTime, jsonSerialize, arginfo_class_MongoDB_BSON_UTCDateTime_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_FE_END
};
