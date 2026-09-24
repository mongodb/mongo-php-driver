/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: ad76e5683d263a0f0f0f07099d651dc8afd25199 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcern___construct, 0, 0, 1)
	ZEND_ARG_TYPE_MASK(0, w, MAY_BE_STRING|MAY_BE_LONG, NULL)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, wtimeout, IS_LONG, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, journal, _IS_BOOL, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcern_getJournal, 0, 0, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_MongoDB_Driver_WriteConcern_getW, 0, 0, MAY_BE_STRING|MAY_BE_LONG|MAY_BE_NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcern_getWtimeout, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcern_isDefault, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcern___set_state, 0, 1, MongoDB\\Driver\\WriteConcern, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcern_bsonSerialize, 0, 0, stdClass, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcern___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

static ZEND_METHOD(MongoDB_Driver_WriteConcern, __construct);
static ZEND_METHOD(MongoDB_Driver_WriteConcern, getJournal);
static ZEND_METHOD(MongoDB_Driver_WriteConcern, getW);
static ZEND_METHOD(MongoDB_Driver_WriteConcern, getWtimeout);
static ZEND_METHOD(MongoDB_Driver_WriteConcern, isDefault);
static ZEND_METHOD(MongoDB_Driver_WriteConcern, __set_state);
static ZEND_METHOD(MongoDB_Driver_WriteConcern, bsonSerialize);
static ZEND_METHOD(MongoDB_Driver_WriteConcern, __unserialize);

static const zend_function_entry class_MongoDB_Driver_WriteConcern_methods[] = {
	ZEND_ME(MongoDB_Driver_WriteConcern, __construct, arginfo_class_MongoDB_Driver_WriteConcern___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcern, getJournal, arginfo_class_MongoDB_Driver_WriteConcern_getJournal, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcern, getW, arginfo_class_MongoDB_Driver_WriteConcern_getW, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcern, getWtimeout, arginfo_class_MongoDB_Driver_WriteConcern_getWtimeout, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcern, isDefault, arginfo_class_MongoDB_Driver_WriteConcern_isDefault, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcern, __set_state, arginfo_class_MongoDB_Driver_WriteConcern___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcern, bsonSerialize, arginfo_class_MongoDB_Driver_WriteConcern_bsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcern, __unserialize, arginfo_class_MongoDB_Driver_WriteConcern___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_WriteConcern(zend_class_entry *class_entry_MongoDB_BSON_Serializable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteConcern", class_MongoDB_Driver_WriteConcern_methods);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);
#else
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
#endif
	zend_class_implements(class_entry, 1, class_entry_MongoDB_BSON_Serializable);

	zval const_MAJORITY_value;
	zend_string *const_MAJORITY_value_str = zend_string_init(PHONGO_WRITE_CONCERN_W_MAJORITY, strlen(PHONGO_WRITE_CONCERN_W_MAJORITY), 1);
	ZVAL_STR(&const_MAJORITY_value, const_MAJORITY_value_str);
	zend_string *const_MAJORITY_name = zend_string_init_interned("MAJORITY", sizeof("MAJORITY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_MAJORITY_name, &const_MAJORITY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_MAJORITY_name);

	zval property_w_default_value;
	ZVAL_UNDEF(&property_w_default_value);
	zend_string *property_w_name = zend_string_init("w", sizeof("w") - 1, 1);
	zend_declare_typed_property(class_entry, property_w_name, &property_w_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING|MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release(property_w_name);

	zval property_j_default_value;
	ZVAL_UNDEF(&property_j_default_value);
	zend_string *property_j_name = zend_string_init("j", sizeof("j") - 1, 1);
	zend_declare_typed_property(class_entry, property_j_name, &property_j_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL|MAY_BE_NULL));
	zend_string_release(property_j_name);

	zval property_wtimeout_default_value;
	ZVAL_UNDEF(&property_wtimeout_default_value);
	zend_string *property_wtimeout_name = zend_string_init("wtimeout", sizeof("wtimeout") - 1, 1);
	zend_declare_typed_property(class_entry, property_wtimeout_name, &property_wtimeout_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_wtimeout_name);

	return class_entry;
}
