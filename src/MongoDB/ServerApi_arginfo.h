/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 44bcb97e14839e750c86309b7b9d2d861db89910 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_ServerApi___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, version, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, strict, _IS_BOOL, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, deprecationErrors, _IS_BOOL, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_ServerApi___set_state, 0, 1, MongoDB\\Driver\\ServerApi, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_ServerApi_bsonSerialize, 0, 0, stdClass, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ServerApi_serialize, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ServerApi_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ServerApi___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ServerApi___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_ServerApi, __construct);
static ZEND_METHOD(MongoDB_Driver_ServerApi, __set_state);
static ZEND_METHOD(MongoDB_Driver_ServerApi, bsonSerialize);
static ZEND_METHOD(MongoDB_Driver_ServerApi, serialize);
static ZEND_METHOD(MongoDB_Driver_ServerApi, unserialize);
static ZEND_METHOD(MongoDB_Driver_ServerApi, __unserialize);
static ZEND_METHOD(MongoDB_Driver_ServerApi, __serialize);


static const zend_function_entry class_MongoDB_Driver_ServerApi_methods[] = {
	ZEND_ME(MongoDB_Driver_ServerApi, __construct, arginfo_class_MongoDB_Driver_ServerApi___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ServerApi, __set_state, arginfo_class_MongoDB_Driver_ServerApi___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ServerApi, bsonSerialize, arginfo_class_MongoDB_Driver_ServerApi_bsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ServerApi, serialize, arginfo_class_MongoDB_Driver_ServerApi_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ServerApi, unserialize, arginfo_class_MongoDB_Driver_ServerApi_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ServerApi, __unserialize, arginfo_class_MongoDB_Driver_ServerApi___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ServerApi, __serialize, arginfo_class_MongoDB_Driver_ServerApi___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_ServerApi(zend_class_entry *class_entry_MongoDB_BSON_Serializable, zend_class_entry *class_entry_Serializable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "ServerApi", class_MongoDB_Driver_ServerApi_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 2, class_entry_MongoDB_BSON_Serializable, class_entry_Serializable);

	zval const_V1_value;
	zend_string *const_V1_value_str = zend_string_init("1", strlen("1"), 1);
	ZVAL_STR(&const_V1_value, const_V1_value_str);
	zend_string *const_V1_name = zend_string_init_interned("V1", sizeof("V1") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_V1_name, &const_V1_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_V1_name);

	return class_entry;
}
