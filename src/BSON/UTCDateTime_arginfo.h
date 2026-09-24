/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: b5033645ba31d4b4e5040548ea979cef5e3e8adb */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___construct, 0, 0, 0)
	ZEND_ARG_OBJ_TYPE_MASK(0, milliseconds, DateTimeInterface|MongoDB\\BSON\\Int64, MAY_BE_LONG|MAY_BE_NULL, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime_toDateTime, 0, 0, DateTime, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime_toDateTimeImmutable, 0, 0, DateTimeImmutable, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___set_state, 0, 1, MongoDB\\BSON\\\125TCDateTime, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_BSON_UTCDateTime, __construct);
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, toDateTime);
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, toDateTimeImmutable);
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, __toString);
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, __set_state);
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, __unserialize);
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, __serialize);
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, jsonSerialize);


static const zend_function_entry class_MongoDB_BSON_UTCDateTime_methods[] = {
	ZEND_ME(MongoDB_BSON_UTCDateTime, __construct, arginfo_class_MongoDB_BSON_UTCDateTime___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, toDateTime, arginfo_class_MongoDB_BSON_UTCDateTime_toDateTime, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, toDateTimeImmutable, arginfo_class_MongoDB_BSON_UTCDateTime_toDateTimeImmutable, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, __toString, arginfo_class_MongoDB_BSON_UTCDateTime___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, __set_state, arginfo_class_MongoDB_BSON_UTCDateTime___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, __unserialize, arginfo_class_MongoDB_BSON_UTCDateTime___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, __serialize, arginfo_class_MongoDB_BSON_UTCDateTime___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, jsonSerialize, arginfo_class_MongoDB_BSON_UTCDateTime_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_UTCDateTime(zend_class_entry *class_entry_MongoDB_BSON_UTCDateTimeInterface, zend_class_entry *class_entry_JsonSerializable, zend_class_entry *class_entry_MongoDB_BSON_Type, zend_class_entry *class_entry_Stringable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "UTCDateTime", class_MongoDB_BSON_UTCDateTime_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 4, class_entry_MongoDB_BSON_UTCDateTimeInterface, class_entry_JsonSerializable, class_entry_MongoDB_BSON_Type, class_entry_Stringable);

	return class_entry;
}
