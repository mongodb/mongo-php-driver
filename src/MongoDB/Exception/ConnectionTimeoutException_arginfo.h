/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 60470413296405ec96e76e4331835d8a27dd5ade */




static const zend_function_entry class_MongoDB_Driver_Exception_ConnectionTimeoutException_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_ConnectionTimeoutException(zend_class_entry *class_entry_MongoDB_Driver_Exception_ConnectionException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "ConnectionTimeoutException", class_MongoDB_Driver_Exception_ConnectionTimeoutException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_ConnectionException);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}
