/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: f4cd2c12699a833caf1cb1d769d564479052180e */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTimeInterface_toDateTime, 0, 0, DateTime, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTimeInterface_toDateTimeImmutable, 0, 0, DateTimeImmutable, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTimeInterface___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()


static const zend_function_entry class_MongoDB_BSON_UTCDateTimeInterface_methods[] = {
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("toDateTime", NULL, arginfo_class_MongoDB_BSON_UTCDateTimeInterface_toDateTime, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("toDateTime", NULL, arginfo_class_MongoDB_BSON_UTCDateTimeInterface_toDateTime, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("toDateTimeImmutable", NULL, arginfo_class_MongoDB_BSON_UTCDateTimeInterface_toDateTimeImmutable, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("toDateTimeImmutable", NULL, arginfo_class_MongoDB_BSON_UTCDateTimeInterface_toDateTimeImmutable, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY("__toString", NULL, arginfo_class_MongoDB_BSON_UTCDateTimeInterface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT, NULL, NULL)
#else
	ZEND_RAW_FENTRY("__toString", NULL, arginfo_class_MongoDB_BSON_UTCDateTimeInterface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_UTCDateTimeInterface(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "UTCDateTimeInterface", class_MongoDB_BSON_UTCDateTimeInterface_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
