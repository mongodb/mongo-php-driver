/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 40e103627b2d12301f87071e4bb06432fe34ef38 */




static const zend_function_entry class_MongoDB_Driver_Exception_ConnectionException_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_ConnectionException(zend_class_entry *class_entry_MongoDB_Driver_Exception_RuntimeException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "ConnectionException", class_MongoDB_Driver_Exception_ConnectionException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_RuntimeException);

	return class_entry;
}
