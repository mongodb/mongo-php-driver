/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: d65173da597c4b41bdc980a3b1e6bbbbe132bc8f */




static const zend_function_entry class_MongoDB_Driver_Exception_ServerException_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_ServerException(zend_class_entry *class_entry_MongoDB_Driver_Exception_RuntimeException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "ServerException", class_MongoDB_Driver_Exception_ServerException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_RuntimeException);

	return class_entry;
}
