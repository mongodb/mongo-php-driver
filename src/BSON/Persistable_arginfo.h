/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 0f9f27e9007f9bf08fe83bdaa3896f333f03af6b */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_class_MongoDB_BSON_Persistable_bsonSerialize, 0, 0, stdClass|MongoDB\\BSON\\Document, MAY_BE_ARRAY)
ZEND_END_ARG_INFO()


static const zend_function_entry class_MongoDB_BSON_Persistable_methods[] = {
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("bsonSerialize", NULL, arginfo_class_MongoDB_BSON_Persistable_bsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("bsonSerialize", NULL, arginfo_class_MongoDB_BSON_Persistable_bsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
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
