/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 72a53fb51ca60f8ccb38a73afe74f76fb81bac71 */

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_RegexInterface_getPattern, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_RegexInterface_getFlags arginfo_class_MongoDB_BSON_RegexInterface_getPattern

#define arginfo_class_MongoDB_BSON_RegexInterface___toString arginfo_class_MongoDB_BSON_RegexInterface_getPattern




static const zend_function_entry class_MongoDB_BSON_RegexInterface_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_RegexInterface, getPattern, arginfo_class_MongoDB_BSON_RegexInterface_getPattern, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_RegexInterface, getFlags, arginfo_class_MongoDB_BSON_RegexInterface_getFlags, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_RegexInterface, __toString, arginfo_class_MongoDB_BSON_RegexInterface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_RegexInterface(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "RegexInterface", class_MongoDB_BSON_RegexInterface_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
