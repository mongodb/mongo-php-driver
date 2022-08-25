/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 9177931b152386567f53fed87887207971acceaf */

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BinaryInterface_getData, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BinaryInterface_getType, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BinaryInterface___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()




static const zend_function_entry class_MongoDB_BSON_BinaryInterface_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_BinaryInterface, getData, arginfo_class_MongoDB_BSON_BinaryInterface_getData, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_BinaryInterface, getType, arginfo_class_MongoDB_BSON_BinaryInterface_getType, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_BinaryInterface, __toString, arginfo_class_MongoDB_BSON_BinaryInterface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_BinaryInterface(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "BinaryInterface", class_MongoDB_BSON_BinaryInterface_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
