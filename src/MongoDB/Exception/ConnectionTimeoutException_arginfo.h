/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 57a9674abe9a302a39515e475f5af803fa3a03d6 */

static zend_class_entry *register_class_MongoDB_Driver_Exception_ConnectionTimeoutException(zend_class_entry *class_entry_MongoDB_Driver_Exception_ConnectionException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "ConnectionTimeoutException", NULL);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_MongoDB_Driver_Exception_ConnectionException, ZEND_ACC_FINAL);
#else
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_ConnectionException);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
#endif

	return class_entry;
}
