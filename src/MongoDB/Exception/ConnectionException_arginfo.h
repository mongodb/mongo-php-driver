/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: bc5696aa261289787b61d29dcdbd0ae8b782c40b */

static zend_class_entry *register_class_MongoDB_Driver_Exception_ConnectionException(zend_class_entry *class_entry_MongoDB_Driver_Exception_RuntimeException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "ConnectionException", NULL);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_MongoDB_Driver_Exception_RuntimeException, 0);
#else
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_RuntimeException);
#endif

	return class_entry;
}
