/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 819affbb75a46cb68d88b46e0c7137590408b010 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Query___construct, 0, 0, 1)
	ZEND_ARG_TYPE_MASK(0, filter, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, queryOptions, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_Query, __construct);


static const zend_function_entry class_MongoDB_Driver_Query_methods[] = {
	ZEND_ME(MongoDB_Driver_Query, __construct, arginfo_class_MongoDB_Driver_Query___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Query(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Query", class_MongoDB_Driver_Query_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;

	return class_entry;
}
