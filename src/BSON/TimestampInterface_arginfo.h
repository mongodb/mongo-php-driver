/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 5ebfb13003e6b82254b7154f492222e0610f0894 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_TimestampInterface_getTimestamp, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_TimestampInterface_getIncrement arginfo_class_MongoDB_BSON_TimestampInterface_getTimestamp

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_TimestampInterface___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()


static const zend_function_entry class_MongoDB_BSON_TimestampInterface_methods[] = {
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("getTimestamp", NULL, arginfo_class_MongoDB_BSON_TimestampInterface_getTimestamp, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("getTimestamp", NULL, arginfo_class_MongoDB_BSON_TimestampInterface_getTimestamp, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("getIncrement", NULL, arginfo_class_MongoDB_BSON_TimestampInterface_getIncrement, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("getIncrement", NULL, arginfo_class_MongoDB_BSON_TimestampInterface_getIncrement, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("__toString", NULL, arginfo_class_MongoDB_BSON_TimestampInterface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("__toString", NULL, arginfo_class_MongoDB_BSON_TimestampInterface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_TimestampInterface(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "TimestampInterface", class_MongoDB_BSON_TimestampInterface_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
