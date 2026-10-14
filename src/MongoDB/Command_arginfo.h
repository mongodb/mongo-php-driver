/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: e779e5022eadeccbd0d2dfb14ce2cf157bb253d8 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Command___construct, 0, 0, 1)
	ZEND_ARG_TYPE_MASK(0, document, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, commandOptions, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

static ZEND_METHOD(MongoDB_Driver_Command, __construct);

static const zend_function_entry class_MongoDB_Driver_Command_methods[] = {
	ZEND_ME(MongoDB_Driver_Command, __construct, arginfo_class_MongoDB_Driver_Command___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Command(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Command", class_MongoDB_Driver_Command_methods);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);
#else
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;
#endif

	return class_entry;
}
