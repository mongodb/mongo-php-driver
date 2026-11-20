/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 1a4f91d780a89d2a50c5239070589470e76e768b */

static zend_class_entry *register_class_MongoDB_Driver_Exception_ExecutionTimeoutException(zend_class_entry *class_entry_MongoDB_Driver_Exception_ServerException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "ExecutionTimeoutException", NULL);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_MongoDB_Driver_Exception_ServerException, ZEND_ACC_FINAL);
#else
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_ServerException);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
#endif

	return class_entry;
}
