/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 63d10707da7b749576ce02f6e565957f94fd7a37 */

static zend_class_entry *register_class_MongoDB_Driver_Exception_AuthenticationException(zend_class_entry *class_entry_MongoDB_Driver_Exception_ConnectionException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "AuthenticationException", NULL);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_MongoDB_Driver_Exception_ConnectionException, 0);
#else
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_ConnectionException);
#endif

	return class_entry;
}
