/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 432323bdeb13c6e1ce5e0201f0957290f412d26d */




static const zend_function_entry class_MongoDB_Driver_Exception_EncryptionException_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_EncryptionException(zend_class_entry *class_entry_MongoDB_Driver_Exception_RuntimeException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "EncryptionException", class_MongoDB_Driver_Exception_EncryptionException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_RuntimeException);

	return class_entry;
}
