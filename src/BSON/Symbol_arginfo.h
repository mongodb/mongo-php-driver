/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 9bd846827a2c2fb7fd6c0cb853a3fec5756073e8 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Symbol___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Symbol___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Symbol___set_state, 0, 1, MongoDB\\BSON\\Symbol, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Symbol___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Symbol___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Symbol_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_BSON_Symbol, __construct);
static ZEND_METHOD(MongoDB_BSON_Symbol, __toString);
static ZEND_METHOD(MongoDB_BSON_Symbol, __set_state);
static ZEND_METHOD(MongoDB_BSON_Symbol, __unserialize);
static ZEND_METHOD(MongoDB_BSON_Symbol, __serialize);
static ZEND_METHOD(MongoDB_BSON_Symbol, jsonSerialize);


static const zend_function_entry class_MongoDB_BSON_Symbol_methods[] = {
	ZEND_ME(MongoDB_BSON_Symbol, __construct, arginfo_class_MongoDB_BSON_Symbol___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Symbol, __toString, arginfo_class_MongoDB_BSON_Symbol___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Symbol, __set_state, arginfo_class_MongoDB_BSON_Symbol___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Symbol, __unserialize, arginfo_class_MongoDB_BSON_Symbol___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Symbol, __serialize, arginfo_class_MongoDB_BSON_Symbol___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Symbol, jsonSerialize, arginfo_class_MongoDB_BSON_Symbol_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Symbol(zend_class_entry *class_entry_JsonSerializable, zend_class_entry *class_entry_MongoDB_BSON_Type, zend_class_entry *class_entry_Stringable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Symbol", class_MongoDB_BSON_Symbol_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 3, class_entry_JsonSerializable, class_entry_MongoDB_BSON_Type, class_entry_Stringable);

	return class_entry;
}
