/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 1e91ce9db35aa0f3b77c3042099b02ac0fb381a9 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_BulkWrite___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_BulkWrite_count, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_BulkWrite_delete, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_MASK(0, filter, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, deleteOptions, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_BulkWrite_insert, 0, 1, IS_MIXED, 0)
	ZEND_ARG_TYPE_MASK(0, document, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_BulkWrite_update, 0, 2, IS_VOID, 0)
	ZEND_ARG_TYPE_MASK(0, filter, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
	ZEND_ARG_TYPE_MASK(0, newObj, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, updateOptions, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_BulkWrite, __construct);
static ZEND_METHOD(MongoDB_Driver_BulkWrite, count);
static ZEND_METHOD(MongoDB_Driver_BulkWrite, delete);
static ZEND_METHOD(MongoDB_Driver_BulkWrite, insert);
static ZEND_METHOD(MongoDB_Driver_BulkWrite, update);


static const zend_function_entry class_MongoDB_Driver_BulkWrite_methods[] = {
	ZEND_ME(MongoDB_Driver_BulkWrite, __construct, arginfo_class_MongoDB_Driver_BulkWrite___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_BulkWrite, count, arginfo_class_MongoDB_Driver_BulkWrite_count, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_Driver_BulkWrite, delete, arginfo_class_MongoDB_Driver_BulkWrite_delete, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_Driver_BulkWrite, insert, arginfo_class_MongoDB_Driver_BulkWrite_insert, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_BulkWrite, update, arginfo_class_MongoDB_Driver_BulkWrite_update, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_BulkWrite(zend_class_entry *class_entry_Countable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "BulkWrite", class_MongoDB_Driver_BulkWrite_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;
	zend_class_implements(class_entry, 1, class_entry_Countable);

	return class_entry;
}
