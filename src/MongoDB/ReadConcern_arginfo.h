/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: d039252f9f7e9b8c30ba4794d488ad7c5cd493d2 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_ReadConcern___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, level, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ReadConcern_getLevel, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ReadConcern_isDefault, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_ReadConcern___set_state, 0, 1, MongoDB\\Driver\\ReadConcern, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_ReadConcern_bsonSerialize, 0, 0, stdClass, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ReadConcern_serialize, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ReadConcern_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ReadConcern___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ReadConcern___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_ReadConcern, __construct);
static ZEND_METHOD(MongoDB_Driver_ReadConcern, getLevel);
static ZEND_METHOD(MongoDB_Driver_ReadConcern, isDefault);
static ZEND_METHOD(MongoDB_Driver_ReadConcern, __set_state);
static ZEND_METHOD(MongoDB_Driver_ReadConcern, bsonSerialize);
static ZEND_METHOD(MongoDB_Driver_ReadConcern, serialize);
static ZEND_METHOD(MongoDB_Driver_ReadConcern, unserialize);
static ZEND_METHOD(MongoDB_Driver_ReadConcern, __unserialize);
static ZEND_METHOD(MongoDB_Driver_ReadConcern, __serialize);


static const zend_function_entry class_MongoDB_Driver_ReadConcern_methods[] = {
	ZEND_ME(MongoDB_Driver_ReadConcern, __construct, arginfo_class_MongoDB_Driver_ReadConcern___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadConcern, getLevel, arginfo_class_MongoDB_Driver_ReadConcern_getLevel, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadConcern, isDefault, arginfo_class_MongoDB_Driver_ReadConcern_isDefault, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadConcern, __set_state, arginfo_class_MongoDB_Driver_ReadConcern___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadConcern, bsonSerialize, arginfo_class_MongoDB_Driver_ReadConcern_bsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadConcern, serialize, arginfo_class_MongoDB_Driver_ReadConcern_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadConcern, unserialize, arginfo_class_MongoDB_Driver_ReadConcern_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadConcern, __unserialize, arginfo_class_MongoDB_Driver_ReadConcern___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadConcern, __serialize, arginfo_class_MongoDB_Driver_ReadConcern___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_ReadConcern(zend_class_entry *class_entry_MongoDB_BSON_Serializable, zend_class_entry *class_entry_Serializable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "ReadConcern", class_MongoDB_Driver_ReadConcern_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 2, class_entry_MongoDB_BSON_Serializable, class_entry_Serializable);

	zval const_LINEARIZABLE_value;
	zend_string *const_LINEARIZABLE_value_str = zend_string_init(MONGOC_READ_CONCERN_LEVEL_LINEARIZABLE, strlen(MONGOC_READ_CONCERN_LEVEL_LINEARIZABLE), 1);
	ZVAL_STR(&const_LINEARIZABLE_value, const_LINEARIZABLE_value_str);
	zend_string *const_LINEARIZABLE_name = zend_string_init_interned("LINEARIZABLE", sizeof("LINEARIZABLE") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_LINEARIZABLE_name, &const_LINEARIZABLE_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_LINEARIZABLE_name);

	zval const_LOCAL_value;
	zend_string *const_LOCAL_value_str = zend_string_init(MONGOC_READ_CONCERN_LEVEL_LOCAL, strlen(MONGOC_READ_CONCERN_LEVEL_LOCAL), 1);
	ZVAL_STR(&const_LOCAL_value, const_LOCAL_value_str);
	zend_string *const_LOCAL_name = zend_string_init_interned("LOCAL", sizeof("LOCAL") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_LOCAL_name, &const_LOCAL_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_LOCAL_name);

	zval const_MAJORITY_value;
	zend_string *const_MAJORITY_value_str = zend_string_init(MONGOC_READ_CONCERN_LEVEL_MAJORITY, strlen(MONGOC_READ_CONCERN_LEVEL_MAJORITY), 1);
	ZVAL_STR(&const_MAJORITY_value, const_MAJORITY_value_str);
	zend_string *const_MAJORITY_name = zend_string_init_interned("MAJORITY", sizeof("MAJORITY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_MAJORITY_name, &const_MAJORITY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_MAJORITY_name);

	zval const_AVAILABLE_value;
	zend_string *const_AVAILABLE_value_str = zend_string_init(MONGOC_READ_CONCERN_LEVEL_AVAILABLE, strlen(MONGOC_READ_CONCERN_LEVEL_AVAILABLE), 1);
	ZVAL_STR(&const_AVAILABLE_value, const_AVAILABLE_value_str);
	zend_string *const_AVAILABLE_name = zend_string_init_interned("AVAILABLE", sizeof("AVAILABLE") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_AVAILABLE_name, &const_AVAILABLE_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_AVAILABLE_name);

	zval const_SNAPSHOT_value;
	zend_string *const_SNAPSHOT_value_str = zend_string_init(MONGOC_READ_CONCERN_LEVEL_SNAPSHOT, strlen(MONGOC_READ_CONCERN_LEVEL_SNAPSHOT), 1);
	ZVAL_STR(&const_SNAPSHOT_value, const_SNAPSHOT_value_str);
	zend_string *const_SNAPSHOT_name = zend_string_init_interned("SNAPSHOT", sizeof("SNAPSHOT") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_SNAPSHOT_name, &const_SNAPSHOT_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_SNAPSHOT_name);

	return class_entry;
}
