/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: f215bf4fccd5214875964d0f557ff2e923b61812 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcernError___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcernError_getCode, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcernError_getInfo, 0, 0, IS_OBJECT, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcernError_getMessage, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

static ZEND_METHOD(MongoDB_Driver_WriteConcernError, __construct);
static ZEND_METHOD(MongoDB_Driver_WriteConcernError, getCode);
static ZEND_METHOD(MongoDB_Driver_WriteConcernError, getInfo);
static ZEND_METHOD(MongoDB_Driver_WriteConcernError, getMessage);

static const zend_function_entry class_MongoDB_Driver_WriteConcernError_methods[] = {
	ZEND_ME(MongoDB_Driver_WriteConcernError, __construct, arginfo_class_MongoDB_Driver_WriteConcernError___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcernError, getCode, arginfo_class_MongoDB_Driver_WriteConcernError_getCode, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcernError, getInfo, arginfo_class_MongoDB_Driver_WriteConcernError_getInfo, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcernError, getMessage, arginfo_class_MongoDB_Driver_WriteConcernError_getMessage, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_WriteConcernError(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteConcernError", class_MongoDB_Driver_WriteConcernError_methods);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);
#else
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;
#endif

	zval property_message_default_value;
	ZVAL_UNDEF(&property_message_default_value);
	zend_declare_typed_property(class_entry, ZSTR_KNOWN(ZEND_STR_MESSAGE), &property_message_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));

	zval property_code_default_value;
	ZVAL_UNDEF(&property_code_default_value);
	zend_declare_typed_property(class_entry, ZSTR_KNOWN(ZEND_STR_CODE), &property_code_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));

	zval property_info_default_value;
	ZVAL_UNDEF(&property_info_default_value);
	zend_string *property_info_name = zend_string_init("info", sizeof("info") - 1, 1);
	zend_declare_typed_property(class_entry, property_info_name, &property_info_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_OBJECT|MAY_BE_NULL));
	zend_string_release(property_info_name);

	return class_entry;
}
