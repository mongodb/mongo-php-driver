/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 93e233be4ba07472af49fe8b8d3278ec30c570a9 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128___set_state, 0, 1, MongoDB\\BSON\\Decimal128, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_BSON_Decimal128, __construct);
static ZEND_METHOD(MongoDB_BSON_Decimal128, __toString);
static ZEND_METHOD(MongoDB_BSON_Decimal128, __set_state);
static ZEND_METHOD(MongoDB_BSON_Decimal128, __unserialize);
static ZEND_METHOD(MongoDB_BSON_Decimal128, __serialize);
static ZEND_METHOD(MongoDB_BSON_Decimal128, jsonSerialize);


static const zend_function_entry class_MongoDB_BSON_Decimal128_methods[] = {
	ZEND_ME(MongoDB_BSON_Decimal128, __construct, arginfo_class_MongoDB_BSON_Decimal128___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Decimal128, __toString, arginfo_class_MongoDB_BSON_Decimal128___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Decimal128, __set_state, arginfo_class_MongoDB_BSON_Decimal128___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Decimal128, __unserialize, arginfo_class_MongoDB_BSON_Decimal128___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Decimal128, __serialize, arginfo_class_MongoDB_BSON_Decimal128___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Decimal128, jsonSerialize, arginfo_class_MongoDB_BSON_Decimal128_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Decimal128(zend_class_entry *class_entry_MongoDB_BSON_Decimal128Interface, zend_class_entry *class_entry_JsonSerializable, zend_class_entry *class_entry_MongoDB_BSON_Type, zend_class_entry *class_entry_Stringable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Decimal128", class_MongoDB_BSON_Decimal128_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 4, class_entry_MongoDB_BSON_Decimal128Interface, class_entry_JsonSerializable, class_entry_MongoDB_BSON_Type, class_entry_Stringable);

	return class_entry;
}
