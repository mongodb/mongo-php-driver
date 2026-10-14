/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 5fb63bbe527f9b0cadf1898ac21e97bbba8dbb6c */

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

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Regex___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Regex___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Regex_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_BSON_Regex, __construct);
static ZEND_METHOD(MongoDB_BSON_Regex, getPattern);
static ZEND_METHOD(MongoDB_BSON_Regex, getFlags);
static ZEND_METHOD(MongoDB_BSON_Regex, __toString);
static ZEND_METHOD(MongoDB_BSON_Regex, __set_state);
static ZEND_METHOD(MongoDB_BSON_Regex, __unserialize);
static ZEND_METHOD(MongoDB_BSON_Regex, __serialize);
static ZEND_METHOD(MongoDB_BSON_Regex, jsonSerialize);


static const zend_function_entry class_MongoDB_BSON_Regex_methods[] = {
	ZEND_ME(MongoDB_BSON_Regex, __construct, arginfo_class_MongoDB_BSON_Regex___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Regex, getPattern, arginfo_class_MongoDB_BSON_Regex_getPattern, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Regex, getFlags, arginfo_class_MongoDB_BSON_Regex_getFlags, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Regex, __toString, arginfo_class_MongoDB_BSON_Regex___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Regex, __set_state, arginfo_class_MongoDB_BSON_Regex___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Regex, __unserialize, arginfo_class_MongoDB_BSON_Regex___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Regex, __serialize, arginfo_class_MongoDB_BSON_Regex___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Regex, jsonSerialize, arginfo_class_MongoDB_BSON_Regex_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Regex(zend_class_entry *class_entry_MongoDB_BSON_RegexInterface, zend_class_entry *class_entry_JsonSerializable, zend_class_entry *class_entry_MongoDB_BSON_Type, zend_class_entry *class_entry_Stringable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Regex", class_MongoDB_BSON_Regex_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 4, class_entry_MongoDB_BSON_RegexInterface, class_entry_JsonSerializable, class_entry_MongoDB_BSON_Type, class_entry_Stringable);

	return class_entry;
}
