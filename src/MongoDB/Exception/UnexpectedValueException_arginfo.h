/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 9c6d689ac6d3ed6b5e2a0dffc38ca423b1d0eea4 */




static const zend_function_entry class_MongoDB_Driver_Exception_UnexpectedValueException_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_UnexpectedValueException(zend_class_entry *class_entry_UnexpectedValueException, zend_class_entry *class_entry_MongoDB_Driver_Exception_Exception)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "UnexpectedValueException", class_MongoDB_Driver_Exception_UnexpectedValueException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_UnexpectedValueException);
	zend_class_implements(class_entry, 1, class_entry_MongoDB_Driver_Exception_Exception);

	return class_entry;
}
