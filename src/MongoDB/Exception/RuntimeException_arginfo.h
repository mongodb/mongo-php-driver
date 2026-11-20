/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: d1165a1dc3d202bd1f29d0b5ffdc336e4faf65d6 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Exception_RuntimeException_hasErrorLabel, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, errorLabel, IS_STRING, 0)
ZEND_END_ARG_INFO()

static ZEND_METHOD(MongoDB_Driver_Exception_RuntimeException, hasErrorLabel);

static const zend_function_entry class_MongoDB_Driver_Exception_RuntimeException_methods[] = {
	ZEND_ME(MongoDB_Driver_Exception_RuntimeException, hasErrorLabel, arginfo_class_MongoDB_Driver_Exception_RuntimeException_hasErrorLabel, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_RuntimeException(zend_class_entry *class_entry_RuntimeException, zend_class_entry *class_entry_MongoDB_Driver_Exception_Exception)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "RuntimeException", class_MongoDB_Driver_Exception_RuntimeException_methods);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_RuntimeException, 0);
#else
	class_entry = zend_register_internal_class_ex(&ce, class_entry_RuntimeException);
#endif
	zend_class_implements(class_entry, 1, class_entry_MongoDB_Driver_Exception_Exception);

	zval property_errorLabels_default_value;
	ZVAL_NULL(&property_errorLabels_default_value);
	zend_string *property_errorLabels_name = zend_string_init("errorLabels", sizeof("errorLabels") - 1, 1);
	zend_declare_typed_property(class_entry, property_errorLabels_name, &property_errorLabels_default_value, ZEND_ACC_PROTECTED, NULL, (zend_type) ZEND_TYPE_INIT_NONE(0));
	zend_string_release(property_errorLabels_name);

	return class_entry;
}
