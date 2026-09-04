/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: f2934f2b5cb4d9407b5bbbf59698a7411c908863 */

static zend_class_entry *register_class_MongoDB_Driver_Exception_UnexpectedValueException(zend_class_entry *class_entry_UnexpectedValueException, zend_class_entry *class_entry_MongoDB_Driver_Exception_Exception)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "UnexpectedValueException", NULL);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_UnexpectedValueException, 0);
#else
	class_entry = zend_register_internal_class_ex(&ce, class_entry_UnexpectedValueException);
#endif
	zend_class_implements(class_entry, 1, class_entry_MongoDB_Driver_Exception_Exception);

	return class_entry;
}
