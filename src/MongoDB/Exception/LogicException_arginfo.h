/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 5ab852c50a988ce0fe2f9f32f5fb2e4b838ab3f9 */

static zend_class_entry *register_class_MongoDB_Driver_Exception_LogicException(zend_class_entry *class_entry_LogicException, zend_class_entry *class_entry_MongoDB_Driver_Exception_Exception)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "LogicException", NULL);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_LogicException, 0);
#else
	class_entry = zend_register_internal_class_ex(&ce, class_entry_LogicException);
#endif
	zend_class_implements(class_entry, 1, class_entry_MongoDB_Driver_Exception_Exception);

	return class_entry;
}
