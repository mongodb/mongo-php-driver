/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 19490ac980b38cd2a6c25c93baf7e1ac0467961f */

static zend_class_entry *register_class_MongoDB_Driver_Exception_ServerException(zend_class_entry *class_entry_MongoDB_Driver_Exception_RuntimeException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "ServerException", NULL);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_MongoDB_Driver_Exception_RuntimeException, 0);
#else
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_RuntimeException);
#endif

	return class_entry;
}
