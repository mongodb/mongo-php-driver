/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 3ecb4933da21ef2b58ca95d74e2aac8d26ca6dc6 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Binary___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, type, IS_LONG, 0, "MongoDB\\BSON\\Binary::TYPE_GENERIC")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Binary_getData, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Binary_getType, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Binary___set_state, 0, 1, MongoDB\\BSON\\Binary, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Binary___toString arginfo_class_MongoDB_BSON_Binary_getData

#define arginfo_class_MongoDB_BSON_Binary_serialize arginfo_class_MongoDB_BSON_Binary_getData

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Binary_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Binary_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Binary___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Binary___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Binary_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Binary_jsonSerialize, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif


static ZEND_METHOD(MongoDB_BSON_Binary, __construct);
static ZEND_METHOD(MongoDB_BSON_Binary, getData);
static ZEND_METHOD(MongoDB_BSON_Binary, getType);
static ZEND_METHOD(MongoDB_BSON_Binary, __set_state);
static ZEND_METHOD(MongoDB_BSON_Binary, __toString);
static ZEND_METHOD(MongoDB_BSON_Binary, serialize);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_Binary, unserialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_Binary, unserialize);
#endif
static ZEND_METHOD(MongoDB_BSON_Binary, __unserialize);
static ZEND_METHOD(MongoDB_BSON_Binary, __serialize);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_Binary, jsonSerialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_Binary, jsonSerialize);
#endif


static const zend_function_entry class_MongoDB_BSON_Binary_methods[] = {
	ZEND_ME(MongoDB_BSON_Binary, __construct, arginfo_class_MongoDB_BSON_Binary___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Binary, getData, arginfo_class_MongoDB_BSON_Binary_getData, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Binary, getType, arginfo_class_MongoDB_BSON_Binary_getType, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Binary, __set_state, arginfo_class_MongoDB_BSON_Binary___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Binary, __toString, arginfo_class_MongoDB_BSON_Binary___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Binary, serialize, arginfo_class_MongoDB_BSON_Binary_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Binary, unserialize, arginfo_class_MongoDB_BSON_Binary_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Binary, unserialize, arginfo_class_MongoDB_BSON_Binary_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_Binary, __unserialize, arginfo_class_MongoDB_BSON_Binary___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Binary, __serialize, arginfo_class_MongoDB_BSON_Binary___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Binary, jsonSerialize, arginfo_class_MongoDB_BSON_Binary_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Binary, jsonSerialize, arginfo_class_MongoDB_BSON_Binary_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Binary(zend_class_entry *class_entry_MongoDB_BSON_BinaryInterface, zend_class_entry *class_entry_JsonSerializable, zend_class_entry *class_entry_MongoDB_BSON_Type, zend_class_entry *class_entry_Serializable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Binary", class_MongoDB_BSON_Binary_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 4, class_entry_MongoDB_BSON_BinaryInterface, class_entry_JsonSerializable, class_entry_MongoDB_BSON_Type, class_entry_Serializable);

	zval const_TYPE_GENERIC_value;
	ZVAL_LONG(&const_TYPE_GENERIC_value, BSON_SUBTYPE_BINARY);
	zend_string *const_TYPE_GENERIC_name = zend_string_init_interned("TYPE_GENERIC", sizeof("TYPE_GENERIC") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_GENERIC_name, &const_TYPE_GENERIC_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_GENERIC_name);

	zval const_TYPE_FUNCTION_value;
	ZVAL_LONG(&const_TYPE_FUNCTION_value, BSON_SUBTYPE_FUNCTION);
	zend_string *const_TYPE_FUNCTION_name = zend_string_init_interned("TYPE_FUNCTION", sizeof("TYPE_FUNCTION") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_FUNCTION_name, &const_TYPE_FUNCTION_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_FUNCTION_name);

	zval const_TYPE_OLD_BINARY_value;
	ZVAL_LONG(&const_TYPE_OLD_BINARY_value, BSON_SUBTYPE_BINARY_DEPRECATED);
	zend_string *const_TYPE_OLD_BINARY_name = zend_string_init_interned("TYPE_OLD_BINARY", sizeof("TYPE_OLD_BINARY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_OLD_BINARY_name, &const_TYPE_OLD_BINARY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_OLD_BINARY_name);

	zval const_TYPE_OLD_UUID_value;
	ZVAL_LONG(&const_TYPE_OLD_UUID_value, BSON_SUBTYPE_UUID_DEPRECATED);
	zend_string *const_TYPE_OLD_UUID_name = zend_string_init_interned("TYPE_OLD_UUID", sizeof("TYPE_OLD_UUID") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_OLD_UUID_name, &const_TYPE_OLD_UUID_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_OLD_UUID_name);

	zval const_TYPE_UUID_value;
	ZVAL_LONG(&const_TYPE_UUID_value, BSON_SUBTYPE_UUID);
	zend_string *const_TYPE_UUID_name = zend_string_init_interned("TYPE_UUID", sizeof("TYPE_UUID") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_UUID_name, &const_TYPE_UUID_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_UUID_name);

	zval const_TYPE_MD5_value;
	ZVAL_LONG(&const_TYPE_MD5_value, BSON_SUBTYPE_MD5);
	zend_string *const_TYPE_MD5_name = zend_string_init_interned("TYPE_MD5", sizeof("TYPE_MD5") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_MD5_name, &const_TYPE_MD5_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_MD5_name);

	zval const_TYPE_ENCRYPTED_value;
	ZVAL_LONG(&const_TYPE_ENCRYPTED_value, BSON_SUBTYPE_ENCRYPTED);
	zend_string *const_TYPE_ENCRYPTED_name = zend_string_init_interned("TYPE_ENCRYPTED", sizeof("TYPE_ENCRYPTED") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_ENCRYPTED_name, &const_TYPE_ENCRYPTED_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_ENCRYPTED_name);

	zval const_TYPE_COLUMN_value;
	ZVAL_LONG(&const_TYPE_COLUMN_value, BSON_SUBTYPE_COLUMN);
	zend_string *const_TYPE_COLUMN_name = zend_string_init_interned("TYPE_COLUMN", sizeof("TYPE_COLUMN") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_COLUMN_name, &const_TYPE_COLUMN_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_COLUMN_name);

	zval const_TYPE_USER_DEFINED_value;
	ZVAL_LONG(&const_TYPE_USER_DEFINED_value, BSON_SUBTYPE_USER);
	zend_string *const_TYPE_USER_DEFINED_name = zend_string_init_interned("TYPE_USER_DEFINED", sizeof("TYPE_USER_DEFINED") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_USER_DEFINED_name, &const_TYPE_USER_DEFINED_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_USER_DEFINED_name);

	return class_entry;
}
