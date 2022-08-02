/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 0e426c19c65154d51a01669619579b7ab2b7f60c */




static const zend_function_entry class_MongoDB_Driver_Exception_Exception_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_Exception(zend_class_entry *class_entry_Throwable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "Exception", class_MongoDB_Driver_Exception_Exception_methods);
	class_entry = zend_register_internal_interface(&ce);
	zend_class_implements(class_entry, 1, class_entry_Throwable);

	return class_entry;
}
