/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 463206b30ddfa600327f4ba731476ab4196939de */




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
