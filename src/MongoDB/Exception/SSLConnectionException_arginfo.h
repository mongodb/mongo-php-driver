/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 95855dc37941494d4bb81c28077c1fa79153d027 */




static const zend_function_entry class_MongoDB_Driver_Exception_SSLConnectionException_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_SSLConnectionException(zend_class_entry *class_entry_MongoDB_Driver_Exception_ConnectionException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "SSLConnectionException", class_MongoDB_Driver_Exception_SSLConnectionException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_ConnectionException);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}
