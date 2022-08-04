/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 8e3bdf5ec19b8267107a1f66c2c31739d935a611 */

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128Interface___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()




static const zend_function_entry class_MongoDB_BSON_Decimal128Interface_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_Decimal128Interface, __toString, arginfo_class_MongoDB_BSON_Decimal128Interface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Decimal128Interface(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Decimal128Interface", class_MongoDB_BSON_Decimal128Interface_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
