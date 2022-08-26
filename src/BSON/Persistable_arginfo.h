/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: babd07f95f47c3b66228ef23b66ab0446cd5c308 */




static const zend_function_entry class_MongoDB_BSON_Persistable_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Persistable(zend_class_entry *class_entry_MongoDB_BSON_Serializable, zend_class_entry *class_entry_MongoDB_BSON_Unserializable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Persistable", class_MongoDB_BSON_Persistable_methods);
	class_entry = zend_register_internal_interface(&ce);
	zend_class_implements(class_entry, 2, class_entry_MongoDB_BSON_Serializable, class_entry_MongoDB_BSON_Unserializable);

	return class_entry;
}
