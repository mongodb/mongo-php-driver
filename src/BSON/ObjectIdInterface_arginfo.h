/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: b68b5dcd9eb41216be44fbcd5fc80349e6fcefd0 */

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_ObjectIdInterface_getTimestamp, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_ObjectIdInterface___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()




static const zend_function_entry class_MongoDB_BSON_ObjectIdInterface_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_ObjectIdInterface, getTimestamp, arginfo_class_MongoDB_BSON_ObjectIdInterface_getTimestamp, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_ObjectIdInterface, __toString, arginfo_class_MongoDB_BSON_ObjectIdInterface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_ObjectIdInterface(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "ObjectIdInterface", class_MongoDB_BSON_ObjectIdInterface_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
