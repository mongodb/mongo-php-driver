/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 20977240e2f1eb527ecb609d61137353a1a5973a */

static zend_class_entry *register_class_MongoDB_Driver_Exception_Exception(zend_class_entry *class_entry_Throwable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "Exception", NULL);
	class_entry = zend_register_internal_interface(&ce);
	zend_class_implements(class_entry, 1, class_entry_Throwable);

	return class_entry;
}
