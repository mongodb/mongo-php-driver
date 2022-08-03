/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: e3bbd956f358c9c22d22617be4d56c4018bd0441 */




static const zend_function_entry class_MongoDB_Driver_Exception_LogicException_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_LogicException(zend_class_entry *class_entry_LogicException, zend_class_entry *class_entry_MongoDB_Driver_Exception_Exception)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "LogicException", class_MongoDB_Driver_Exception_LogicException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_LogicException);
	zend_class_implements(class_entry, 1, class_entry_MongoDB_Driver_Exception_Exception);

	return class_entry;
}
