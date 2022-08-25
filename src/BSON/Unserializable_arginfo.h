/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 2954181cca6d68d40bf46d5ffad15f3ccbc73b72 */

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Unserializable_bsonUnserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()




static const zend_function_entry class_MongoDB_BSON_Unserializable_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_Unserializable, bsonUnserialize, arginfo_class_MongoDB_BSON_Unserializable_bsonUnserialize, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Unserializable(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Unserializable", class_MongoDB_BSON_Unserializable_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
