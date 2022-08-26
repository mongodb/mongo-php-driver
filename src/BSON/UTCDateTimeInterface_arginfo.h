/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: d6725fe1f1e80ebc13fcc924b0a794b23b44c034 */

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTimeInterface_toDateTime, 0, 0, DateTime, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTimeInterface___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()




static const zend_function_entry class_MongoDB_BSON_UTCDateTimeInterface_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_UTCDateTimeInterface, toDateTime, arginfo_class_MongoDB_BSON_UTCDateTimeInterface_toDateTime, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_UTCDateTimeInterface, __toString, arginfo_class_MongoDB_BSON_UTCDateTimeInterface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_UTCDateTimeInterface(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "UTCDateTimeInterface", class_MongoDB_BSON_UTCDateTimeInterface_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
