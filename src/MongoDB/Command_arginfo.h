/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: e613d505cab7a354e0e6b1e0395933169891021f */

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
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;

	return class_entry;
}
