/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: e152c0d8d8a6b9d5cd6f43479fb32972ecd585d5 */




static const zend_function_entry class_MongoDB_Driver_Exception_ExecutionTimeoutException_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_ExecutionTimeoutException(zend_class_entry *class_entry_MongoDB_Driver_Exception_ServerException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "ExecutionTimeoutException", class_MongoDB_Driver_Exception_ExecutionTimeoutException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_ServerException);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}
