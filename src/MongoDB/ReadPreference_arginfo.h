/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 749f4ec07a374c2d9abb2dcb4cb1e7204d52923d */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_ReadPreference___construct, 0, 0, 1)
	ZEND_ARG_TYPE_MASK(0, mode, MAY_BE_STRING|MAY_BE_LONG, NULL)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, tagSets, IS_ARRAY, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ReadPreference_getHedge, 0, 0, IS_OBJECT, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ReadPreference_getMaxStalenessSeconds, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_ReadPreference_getMode arginfo_class_MongoDB_Driver_ReadPreference_getMaxStalenessSeconds

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ReadPreference_getModeString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ReadPreference_getTagSets, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_ReadPreference___set_state, 0, 1, MongoDB\\Driver\\ReadPreference, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_ReadPreference_bsonSerialize, 0, 0, stdClass, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_ReadPreference_serialize arginfo_class_MongoDB_Driver_ReadPreference_getModeString

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ReadPreference_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ReadPreference___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_ReadPreference___serialize arginfo_class_MongoDB_Driver_ReadPreference_getTagSets


static ZEND_METHOD(MongoDB_Driver_ReadPreference, __construct);
static ZEND_METHOD(MongoDB_Driver_ReadPreference, getHedge);
static ZEND_METHOD(MongoDB_Driver_ReadPreference, getMaxStalenessSeconds);
static ZEND_METHOD(MongoDB_Driver_ReadPreference, getMode);
static ZEND_METHOD(MongoDB_Driver_ReadPreference, getModeString);
static ZEND_METHOD(MongoDB_Driver_ReadPreference, getTagSets);
static ZEND_METHOD(MongoDB_Driver_ReadPreference, __set_state);
static ZEND_METHOD(MongoDB_Driver_ReadPreference, bsonSerialize);
static ZEND_METHOD(MongoDB_Driver_ReadPreference, serialize);
static ZEND_METHOD(MongoDB_Driver_ReadPreference, unserialize);
static ZEND_METHOD(MongoDB_Driver_ReadPreference, __unserialize);
static ZEND_METHOD(MongoDB_Driver_ReadPreference, __serialize);


static const zend_function_entry class_MongoDB_Driver_ReadPreference_methods[] = {
	ZEND_ME(MongoDB_Driver_ReadPreference, __construct, arginfo_class_MongoDB_Driver_ReadPreference___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadPreference, getHedge, arginfo_class_MongoDB_Driver_ReadPreference_getHedge, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadPreference, getMaxStalenessSeconds, arginfo_class_MongoDB_Driver_ReadPreference_getMaxStalenessSeconds, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadPreference, getMode, arginfo_class_MongoDB_Driver_ReadPreference_getMode, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL|ZEND_ACC_DEPRECATED)
	ZEND_ME(MongoDB_Driver_ReadPreference, getModeString, arginfo_class_MongoDB_Driver_ReadPreference_getModeString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadPreference, getTagSets, arginfo_class_MongoDB_Driver_ReadPreference_getTagSets, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadPreference, __set_state, arginfo_class_MongoDB_Driver_ReadPreference___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadPreference, bsonSerialize, arginfo_class_MongoDB_Driver_ReadPreference_bsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadPreference, serialize, arginfo_class_MongoDB_Driver_ReadPreference_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadPreference, unserialize, arginfo_class_MongoDB_Driver_ReadPreference_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadPreference, __unserialize, arginfo_class_MongoDB_Driver_ReadPreference___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ReadPreference, __serialize, arginfo_class_MongoDB_Driver_ReadPreference___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_ReadPreference(zend_class_entry *class_entry_MongoDB_BSON_Serializable, zend_class_entry *class_entry_Serializable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "ReadPreference", class_MongoDB_Driver_ReadPreference_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 2, class_entry_MongoDB_BSON_Serializable, class_entry_Serializable);

	zval const_RP_PRIMARY_value;
	ZVAL_LONG(&const_RP_PRIMARY_value, MONGOC_READ_PRIMARY);
	zend_string *const_RP_PRIMARY_name = zend_string_init_interned("RP_PRIMARY", sizeof("RP_PRIMARY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_RP_PRIMARY_name, &const_RP_PRIMARY_value, ZEND_ACC_PUBLIC|ZEND_ACC_DEPRECATED, NULL);
	zend_string_release(const_RP_PRIMARY_name);

	zval const_RP_PRIMARY_PREFERRED_value;
	ZVAL_LONG(&const_RP_PRIMARY_PREFERRED_value, MONGOC_READ_PRIMARY_PREFERRED);
	zend_string *const_RP_PRIMARY_PREFERRED_name = zend_string_init_interned("RP_PRIMARY_PREFERRED", sizeof("RP_PRIMARY_PREFERRED") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_RP_PRIMARY_PREFERRED_name, &const_RP_PRIMARY_PREFERRED_value, ZEND_ACC_PUBLIC|ZEND_ACC_DEPRECATED, NULL);
	zend_string_release(const_RP_PRIMARY_PREFERRED_name);

	zval const_RP_SECONDARY_value;
	ZVAL_LONG(&const_RP_SECONDARY_value, MONGOC_READ_SECONDARY);
	zend_string *const_RP_SECONDARY_name = zend_string_init_interned("RP_SECONDARY", sizeof("RP_SECONDARY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_RP_SECONDARY_name, &const_RP_SECONDARY_value, ZEND_ACC_PUBLIC|ZEND_ACC_DEPRECATED, NULL);
	zend_string_release(const_RP_SECONDARY_name);

	zval const_RP_SECONDARY_PREFERRED_value;
	ZVAL_LONG(&const_RP_SECONDARY_PREFERRED_value, MONGOC_READ_SECONDARY_PREFERRED);
	zend_string *const_RP_SECONDARY_PREFERRED_name = zend_string_init_interned("RP_SECONDARY_PREFERRED", sizeof("RP_SECONDARY_PREFERRED") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_RP_SECONDARY_PREFERRED_name, &const_RP_SECONDARY_PREFERRED_value, ZEND_ACC_PUBLIC|ZEND_ACC_DEPRECATED, NULL);
	zend_string_release(const_RP_SECONDARY_PREFERRED_name);

	zval const_RP_NEAREST_value;
	ZVAL_LONG(&const_RP_NEAREST_value, MONGOC_READ_NEAREST);
	zend_string *const_RP_NEAREST_name = zend_string_init_interned("RP_NEAREST", sizeof("RP_NEAREST") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_RP_NEAREST_name, &const_RP_NEAREST_value, ZEND_ACC_PUBLIC|ZEND_ACC_DEPRECATED, NULL);
	zend_string_release(const_RP_NEAREST_name);

	zval const_PRIMARY_value;
	zend_string *const_PRIMARY_value_str = zend_string_init(PHONGO_READ_PRIMARY, strlen(PHONGO_READ_PRIMARY), 1);
	ZVAL_STR(&const_PRIMARY_value, const_PRIMARY_value_str);
	zend_string *const_PRIMARY_name = zend_string_init_interned("PRIMARY", sizeof("PRIMARY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_PRIMARY_name, &const_PRIMARY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_PRIMARY_name);

	zval const_PRIMARY_PREFERRED_value;
	zend_string *const_PRIMARY_PREFERRED_value_str = zend_string_init(PHONGO_READ_PRIMARY_PREFERRED, strlen(PHONGO_READ_PRIMARY_PREFERRED), 1);
	ZVAL_STR(&const_PRIMARY_PREFERRED_value, const_PRIMARY_PREFERRED_value_str);
	zend_string *const_PRIMARY_PREFERRED_name = zend_string_init_interned("PRIMARY_PREFERRED", sizeof("PRIMARY_PREFERRED") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_PRIMARY_PREFERRED_name, &const_PRIMARY_PREFERRED_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_PRIMARY_PREFERRED_name);

	zval const_SECONDARY_value;
	zend_string *const_SECONDARY_value_str = zend_string_init(PHONGO_READ_SECONDARY, strlen(PHONGO_READ_SECONDARY), 1);
	ZVAL_STR(&const_SECONDARY_value, const_SECONDARY_value_str);
	zend_string *const_SECONDARY_name = zend_string_init_interned("SECONDARY", sizeof("SECONDARY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_SECONDARY_name, &const_SECONDARY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_SECONDARY_name);

	zval const_SECONDARY_PREFERRED_value;
	zend_string *const_SECONDARY_PREFERRED_value_str = zend_string_init(PHONGO_READ_SECONDARY_PREFERRED, strlen(PHONGO_READ_SECONDARY_PREFERRED), 1);
	ZVAL_STR(&const_SECONDARY_PREFERRED_value, const_SECONDARY_PREFERRED_value_str);
	zend_string *const_SECONDARY_PREFERRED_name = zend_string_init_interned("SECONDARY_PREFERRED", sizeof("SECONDARY_PREFERRED") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_SECONDARY_PREFERRED_name, &const_SECONDARY_PREFERRED_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_SECONDARY_PREFERRED_name);

	zval const_NEAREST_value;
	zend_string *const_NEAREST_value_str = zend_string_init(PHONGO_READ_NEAREST, strlen(PHONGO_READ_NEAREST), 1);
	ZVAL_STR(&const_NEAREST_value, const_NEAREST_value_str);
	zend_string *const_NEAREST_name = zend_string_init_interned("NEAREST", sizeof("NEAREST") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_NEAREST_name, &const_NEAREST_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_NEAREST_name);

	zval const_NO_MAX_STALENESS_value;
	ZVAL_LONG(&const_NO_MAX_STALENESS_value, MONGOC_NO_MAX_STALENESS);
	zend_string *const_NO_MAX_STALENESS_name = zend_string_init_interned("NO_MAX_STALENESS", sizeof("NO_MAX_STALENESS") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_NO_MAX_STALENESS_name, &const_NO_MAX_STALENESS_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_NO_MAX_STALENESS_name);

	zval const_SMALLEST_MAX_STALENESS_SECONDS_value;
	ZVAL_LONG(&const_SMALLEST_MAX_STALENESS_SECONDS_value, MONGOC_SMALLEST_MAX_STALENESS_SECONDS);
	zend_string *const_SMALLEST_MAX_STALENESS_SECONDS_name = zend_string_init_interned("SMALLEST_MAX_STALENESS_SECONDS", sizeof("SMALLEST_MAX_STALENESS_SECONDS") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_SMALLEST_MAX_STALENESS_SECONDS_name, &const_SMALLEST_MAX_STALENESS_SECONDS_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_SMALLEST_MAX_STALENESS_SECONDS_name);

	return class_entry;
}
