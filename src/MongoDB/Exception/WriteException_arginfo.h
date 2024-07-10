/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: bf7229dbad041a8fe73af62d1961700783a933e3 */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Exception_WriteException_getWriteResult, 0, 0, MongoDB\\Driver\\WriteResult, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_Exception_WriteException, getWriteResult);


static const zend_function_entry class_MongoDB_Driver_Exception_WriteException_methods[] = {
	ZEND_ME(MongoDB_Driver_Exception_WriteException, getWriteResult, arginfo_class_MongoDB_Driver_Exception_WriteException_getWriteResult, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_WriteException(zend_class_entry *class_entry_MongoDB_Driver_Exception_RuntimeException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "WriteException", class_MongoDB_Driver_Exception_WriteException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_RuntimeException);
	class_entry->ce_flags |= ZEND_ACC_ABSTRACT|ZEND_ACC_DEPRECATED;

	zval property_writeResult_default_value;
	ZVAL_NULL(&property_writeResult_default_value);
	zend_string *property_writeResult_name = zend_string_init("writeResult", sizeof("writeResult") - 1, 1);
	zend_declare_property_ex(class_entry, property_writeResult_name, &property_writeResult_default_value, ZEND_ACC_PROTECTED, NULL);
	zend_string_release(property_writeResult_name);

	return class_entry;
}
