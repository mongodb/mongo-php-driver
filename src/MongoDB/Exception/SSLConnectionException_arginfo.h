/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 2948889569c0558ad634e3d05360b9d942dbb282 */




static const zend_function_entry class_MongoDB_Driver_Exception_SSLConnectionException_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_SSLConnectionException(zend_class_entry *class_entry_MongoDB_Driver_Exception_ConnectionException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "SSLConnectionException", class_MongoDB_Driver_Exception_SSLConnectionException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_ConnectionException);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_DEPRECATED;

	return class_entry;
}
