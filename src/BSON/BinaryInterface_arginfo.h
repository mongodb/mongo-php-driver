/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 6bfe5c53bac9804fdb1498594a81962cf1b6c6c7 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BinaryInterface_getData, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BinaryInterface_getType, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_BinaryInterface___toString arginfo_class_MongoDB_BSON_BinaryInterface_getData


static const zend_function_entry class_MongoDB_BSON_BinaryInterface_methods[] = {
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("getData", NULL, arginfo_class_MongoDB_BSON_BinaryInterface_getData, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("getData", NULL, arginfo_class_MongoDB_BSON_BinaryInterface_getData, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("getType", NULL, arginfo_class_MongoDB_BSON_BinaryInterface_getType, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("getType", NULL, arginfo_class_MongoDB_BSON_BinaryInterface_getType, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("__toString", NULL, arginfo_class_MongoDB_BSON_BinaryInterface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("__toString", NULL, arginfo_class_MongoDB_BSON_BinaryInterface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_BinaryInterface(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "BinaryInterface", class_MongoDB_BSON_BinaryInterface_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
