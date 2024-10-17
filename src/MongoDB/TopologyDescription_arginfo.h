/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 2cd175f4e81e332cd83adb867d51db77907ee8c7 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_TopologyDescription___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_TopologyDescription_getServers, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_TopologyDescription_getType, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_TopologyDescription_hasReadableServer, 0, 0, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, readPreference, MongoDB\\Driver\\ReadPreference, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_TopologyDescription_hasWritableServer, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_TopologyDescription, __construct);
static ZEND_METHOD(MongoDB_Driver_TopologyDescription, getServers);
static ZEND_METHOD(MongoDB_Driver_TopologyDescription, getType);
static ZEND_METHOD(MongoDB_Driver_TopologyDescription, hasReadableServer);
static ZEND_METHOD(MongoDB_Driver_TopologyDescription, hasWritableServer);


static const zend_function_entry class_MongoDB_Driver_TopologyDescription_methods[] = {
	ZEND_ME(MongoDB_Driver_TopologyDescription, __construct, arginfo_class_MongoDB_Driver_TopologyDescription___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_TopologyDescription, getServers, arginfo_class_MongoDB_Driver_TopologyDescription_getServers, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_TopologyDescription, getType, arginfo_class_MongoDB_Driver_TopologyDescription_getType, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_TopologyDescription, hasReadableServer, arginfo_class_MongoDB_Driver_TopologyDescription_hasReadableServer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_TopologyDescription, hasWritableServer, arginfo_class_MongoDB_Driver_TopologyDescription_hasWritableServer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_TopologyDescription(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "TopologyDescription", class_MongoDB_Driver_TopologyDescription_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;

	zval const_TYPE_UNKNOWN_value;
	zend_string *const_TYPE_UNKNOWN_value_str = zend_string_init(PHONGO_TOPOLOGY_UNKNOWN, strlen(PHONGO_TOPOLOGY_UNKNOWN), 1);
	ZVAL_STR(&const_TYPE_UNKNOWN_value, const_TYPE_UNKNOWN_value_str);
	zend_string *const_TYPE_UNKNOWN_name = zend_string_init_interned("TYPE_UNKNOWN", sizeof("TYPE_UNKNOWN") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_UNKNOWN_name, &const_TYPE_UNKNOWN_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_UNKNOWN_name);

	zval const_TYPE_SINGLE_value;
	zend_string *const_TYPE_SINGLE_value_str = zend_string_init(PHONGO_TOPOLOGY_SINGLE, strlen(PHONGO_TOPOLOGY_SINGLE), 1);
	ZVAL_STR(&const_TYPE_SINGLE_value, const_TYPE_SINGLE_value_str);
	zend_string *const_TYPE_SINGLE_name = zend_string_init_interned("TYPE_SINGLE", sizeof("TYPE_SINGLE") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_SINGLE_name, &const_TYPE_SINGLE_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_SINGLE_name);

	zval const_TYPE_SHARDED_value;
	zend_string *const_TYPE_SHARDED_value_str = zend_string_init(PHONGO_TOPOLOGY_SHARDED, strlen(PHONGO_TOPOLOGY_SHARDED), 1);
	ZVAL_STR(&const_TYPE_SHARDED_value, const_TYPE_SHARDED_value_str);
	zend_string *const_TYPE_SHARDED_name = zend_string_init_interned("TYPE_SHARDED", sizeof("TYPE_SHARDED") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_SHARDED_name, &const_TYPE_SHARDED_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_SHARDED_name);

	zval const_TYPE_REPLICA_SET_NO_PRIMARY_value;
	zend_string *const_TYPE_REPLICA_SET_NO_PRIMARY_value_str = zend_string_init(PHONGO_TOPOLOGY_REPLICA_SET_NO_PRIMARY, strlen(PHONGO_TOPOLOGY_REPLICA_SET_NO_PRIMARY), 1);
	ZVAL_STR(&const_TYPE_REPLICA_SET_NO_PRIMARY_value, const_TYPE_REPLICA_SET_NO_PRIMARY_value_str);
	zend_string *const_TYPE_REPLICA_SET_NO_PRIMARY_name = zend_string_init_interned("TYPE_REPLICA_SET_NO_PRIMARY", sizeof("TYPE_REPLICA_SET_NO_PRIMARY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_REPLICA_SET_NO_PRIMARY_name, &const_TYPE_REPLICA_SET_NO_PRIMARY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_REPLICA_SET_NO_PRIMARY_name);

	zval const_TYPE_REPLICA_SET_WITH_PRIMARY_value;
	zend_string *const_TYPE_REPLICA_SET_WITH_PRIMARY_value_str = zend_string_init(PHONGO_TOPOLOGY_REPLICA_SET_WITH_PRIMARY, strlen(PHONGO_TOPOLOGY_REPLICA_SET_WITH_PRIMARY), 1);
	ZVAL_STR(&const_TYPE_REPLICA_SET_WITH_PRIMARY_value, const_TYPE_REPLICA_SET_WITH_PRIMARY_value_str);
	zend_string *const_TYPE_REPLICA_SET_WITH_PRIMARY_name = zend_string_init_interned("TYPE_REPLICA_SET_WITH_PRIMARY", sizeof("TYPE_REPLICA_SET_WITH_PRIMARY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_REPLICA_SET_WITH_PRIMARY_name, &const_TYPE_REPLICA_SET_WITH_PRIMARY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_REPLICA_SET_WITH_PRIMARY_name);

	zval const_TYPE_LOAD_BALANCED_value;
	zend_string *const_TYPE_LOAD_BALANCED_value_str = zend_string_init(PHONGO_TOPOLOGY_LOAD_BALANCED, strlen(PHONGO_TOPOLOGY_LOAD_BALANCED), 1);
	ZVAL_STR(&const_TYPE_LOAD_BALANCED_value, const_TYPE_LOAD_BALANCED_value_str);
	zend_string *const_TYPE_LOAD_BALANCED_name = zend_string_init_interned("TYPE_LOAD_BALANCED", sizeof("TYPE_LOAD_BALANCED") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_LOAD_BALANCED_name, &const_TYPE_LOAD_BALANCED_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_LOAD_BALANCED_name);

	return class_entry;
}
