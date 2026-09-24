/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: fd63ec36126fe9b35fb30affbcd4a0861b5c85f8 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Decimal128Interface___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()


static const zend_function_entry class_MongoDB_BSON_Decimal128Interface_methods[] = {
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("__toString", NULL, arginfo_class_MongoDB_BSON_Decimal128Interface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("__toString", NULL, arginfo_class_MongoDB_BSON_Decimal128Interface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Decimal128Interface(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Decimal128Interface", class_MongoDB_BSON_Decimal128Interface_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
