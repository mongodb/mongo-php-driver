/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: cbb517e1d922625ff18bdc81620686a44a87e85c */

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_MASK_EX(arginfo_class_MongoDB_BSON_Serializable_bsonSerialize, 0, 0, MAY_BE_ARRAY|MAY_BE_OBJECT)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Serializable_bsonSerialize, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif


#if PHP_VERSION_ID >= 80000
#endif
#if !(PHP_VERSION_ID >= 80000)
#endif


static const zend_function_entry class_MongoDB_BSON_Serializable_methods[] = {
#if PHP_VERSION_ID >= 80000
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_Serializable, bsonSerialize, arginfo_class_MongoDB_BSON_Serializable_bsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_Serializable, bsonSerialize, arginfo_class_MongoDB_BSON_Serializable_bsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Serializable(zend_class_entry *class_entry_MongoDB_BSON_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Serializable", class_MongoDB_BSON_Serializable_methods);
	class_entry = zend_register_internal_interface(&ce);
	zend_class_implements(class_entry, 1, class_entry_MongoDB_BSON_Type);

	return class_entry;
}
