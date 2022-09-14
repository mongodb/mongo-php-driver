/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 948dccc4736ffc466b4a7d3b3e59f1e6cca9a0bb */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PersistableEnum_bsonSerialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PersistableEnum_bsonUnserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(MongoDB_BSON_PersistableEnum, bsonSerialize);
ZEND_METHOD(MongoDB_BSON_PersistableEnum, bsonUnserialize);


static const zend_function_entry class_MongoDB_BSON_PersistableEnum_methods[] = {
	ZEND_ME(MongoDB_BSON_PersistableEnum, bsonSerialize, arginfo_class_MongoDB_BSON_PersistableEnum_bsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PersistableEnum, bsonUnserialize, arginfo_class_MongoDB_BSON_PersistableEnum_bsonUnserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_PersistableEnum(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "PersistableEnum", class_MongoDB_BSON_PersistableEnum_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_TRAIT;

	return class_entry;
}
