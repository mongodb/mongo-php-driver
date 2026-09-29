/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 56ea9546c171aa438abb57626f0129fe473339fb */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Unserializable_bsonUnserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


static const zend_function_entry class_MongoDB_BSON_Unserializable_methods[] = {
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("bsonUnserialize", NULL, arginfo_class_MongoDB_BSON_Unserializable_bsonUnserialize, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("bsonUnserialize", NULL, arginfo_class_MongoDB_BSON_Unserializable_bsonUnserialize, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Unserializable(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Unserializable", class_MongoDB_BSON_Unserializable_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
