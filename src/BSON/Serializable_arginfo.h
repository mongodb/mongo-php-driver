/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: cf790c4aaa9d12777681842564139008ec73218b */

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_OBJ_TYPE_MASK_EX(arginfo_class_MongoDB_BSON_Serializable_bsonSerialize, 0, 0, stdClass|MongoDB\\BSON\\Document|MongoDB\\BSON\\PackedArray, MAY_BE_ARRAY)
ZEND_END_ARG_INFO()




static const zend_function_entry class_MongoDB_BSON_Serializable_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_Serializable, bsonSerialize, arginfo_class_MongoDB_BSON_Serializable_bsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
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
