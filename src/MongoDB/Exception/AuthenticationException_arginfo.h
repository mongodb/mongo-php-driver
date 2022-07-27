/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: fe7a688ceb0d1279b05338f8c92c4cbdfe6ff80b */




static const zend_function_entry class_MongoDB_Driver_Exception_AuthenticationException_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_AuthenticationException(zend_class_entry *class_entry_MongoDB_Driver_Exception_ConnectionException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "AuthenticationException", class_MongoDB_Driver_Exception_AuthenticationException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_ConnectionException);

	return class_entry;
}
