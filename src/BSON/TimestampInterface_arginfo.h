/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 2a749d00f223cb25242c2f3a303b183074f5841c */

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_TimestampInterface_getTimestamp, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_TimestampInterface_getIncrement arginfo_class_MongoDB_BSON_TimestampInterface_getTimestamp

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_TimestampInterface___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()




static const zend_function_entry class_MongoDB_BSON_TimestampInterface_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_TimestampInterface, getTimestamp, arginfo_class_MongoDB_BSON_TimestampInterface_getTimestamp, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_TimestampInterface, getIncrement, arginfo_class_MongoDB_BSON_TimestampInterface_getIncrement, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_TimestampInterface, __toString, arginfo_class_MongoDB_BSON_TimestampInterface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_TimestampInterface(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "TimestampInterface", class_MongoDB_BSON_TimestampInterface_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
