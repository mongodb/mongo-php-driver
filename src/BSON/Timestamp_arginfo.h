/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 0b768cfa7860b1bde5cbc32742528dd5754b9a85 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp___construct, 0, 0, 2)
	ZEND_ARG_TYPE_MASK(0, increment, MAY_BE_LONG|MAY_BE_STRING, NULL)
	ZEND_ARG_TYPE_MASK(0, timestamp, MAY_BE_LONG|MAY_BE_STRING, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp_getTimestamp, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Timestamp_getIncrement arginfo_class_MongoDB_BSON_Timestamp_getTimestamp

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp___set_state, 0, 1, MongoDB\\BSON\\Timestamp, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Timestamp_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_BSON_Timestamp, __construct);
static ZEND_METHOD(MongoDB_BSON_Timestamp, getTimestamp);
static ZEND_METHOD(MongoDB_BSON_Timestamp, getIncrement);
static ZEND_METHOD(MongoDB_BSON_Timestamp, __toString);
static ZEND_METHOD(MongoDB_BSON_Timestamp, __set_state);
static ZEND_METHOD(MongoDB_BSON_Timestamp, __unserialize);
static ZEND_METHOD(MongoDB_BSON_Timestamp, __serialize);
static ZEND_METHOD(MongoDB_BSON_Timestamp, jsonSerialize);


static const zend_function_entry class_MongoDB_BSON_Timestamp_methods[] = {
	ZEND_ME(MongoDB_BSON_Timestamp, __construct, arginfo_class_MongoDB_BSON_Timestamp___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Timestamp, getTimestamp, arginfo_class_MongoDB_BSON_Timestamp_getTimestamp, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Timestamp, getIncrement, arginfo_class_MongoDB_BSON_Timestamp_getIncrement, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Timestamp, __toString, arginfo_class_MongoDB_BSON_Timestamp___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Timestamp, __set_state, arginfo_class_MongoDB_BSON_Timestamp___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Timestamp, __unserialize, arginfo_class_MongoDB_BSON_Timestamp___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Timestamp, __serialize, arginfo_class_MongoDB_BSON_Timestamp___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Timestamp, jsonSerialize, arginfo_class_MongoDB_BSON_Timestamp_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Timestamp(zend_class_entry *class_entry_MongoDB_BSON_TimestampInterface, zend_class_entry *class_entry_JsonSerializable, zend_class_entry *class_entry_MongoDB_BSON_Type, zend_class_entry *class_entry_Stringable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Timestamp", class_MongoDB_BSON_Timestamp_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 4, class_entry_MongoDB_BSON_TimestampInterface, class_entry_JsonSerializable, class_entry_MongoDB_BSON_Type, class_entry_Stringable);

	return class_entry;
}
