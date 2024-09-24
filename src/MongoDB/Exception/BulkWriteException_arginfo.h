/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 8d6c350124574a892c9bbe1c8fda8b0a55e09a8b */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Exception_BulkWriteException_getWriteResult, 0, 0, MongoDB\\Driver\\WriteResult, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_Exception_BulkWriteException, getWriteResult);


static const zend_function_entry class_MongoDB_Driver_Exception_BulkWriteException_methods[] = {
	ZEND_ME(MongoDB_Driver_Exception_BulkWriteException, getWriteResult, arginfo_class_MongoDB_Driver_Exception_BulkWriteException_getWriteResult, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_BulkWriteException(zend_class_entry *class_entry_MongoDB_Driver_Exception_RuntimeException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "BulkWriteException", class_MongoDB_Driver_Exception_BulkWriteException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_RuntimeException);

	zval property_writeResult_default_value;
	ZVAL_NULL(&property_writeResult_default_value);
	zend_string *property_writeResult_name = zend_string_init("writeResult", sizeof("writeResult") - 1, 1);
	zend_declare_typed_property(class_entry, property_writeResult_name, &property_writeResult_default_value, ZEND_ACC_PROTECTED, NULL, (zend_type) ZEND_TYPE_INIT_NONE(0));
	zend_string_release(property_writeResult_name);

	return class_entry;
}
