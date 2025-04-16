/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 2af15e87d1d8095137d315218e688221ac2b5983 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_BulkWriteCommand___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_BulkWriteCommand_count, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_BulkWriteCommand_deleteOne, 0, 2, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, namespace, IS_STRING, 0)
	ZEND_ARG_TYPE_MASK(0, filter, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_BulkWriteCommand_deleteMany arginfo_class_MongoDB_Driver_BulkWriteCommand_deleteOne

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_BulkWriteCommand_insertOne, 0, 2, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, namespace, IS_STRING, 0)
	ZEND_ARG_TYPE_MASK(0, document, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_BulkWriteCommand_replaceOne, 0, 3, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, namespace, IS_STRING, 0)
	ZEND_ARG_TYPE_MASK(0, filter, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
	ZEND_ARG_TYPE_MASK(0, replacement, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_BulkWriteCommand_updateOne, 0, 3, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, namespace, IS_STRING, 0)
	ZEND_ARG_TYPE_MASK(0, filter, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
	ZEND_ARG_TYPE_MASK(0, update, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_BulkWriteCommand_updateMany arginfo_class_MongoDB_Driver_BulkWriteCommand_updateOne


static ZEND_METHOD(MongoDB_Driver_BulkWriteCommand, __construct);
static ZEND_METHOD(MongoDB_Driver_BulkWriteCommand, count);
static ZEND_METHOD(MongoDB_Driver_BulkWriteCommand, deleteOne);
static ZEND_METHOD(MongoDB_Driver_BulkWriteCommand, deleteMany);
static ZEND_METHOD(MongoDB_Driver_BulkWriteCommand, insertOne);
static ZEND_METHOD(MongoDB_Driver_BulkWriteCommand, replaceOne);
static ZEND_METHOD(MongoDB_Driver_BulkWriteCommand, updateOne);
static ZEND_METHOD(MongoDB_Driver_BulkWriteCommand, updateMany);


static const zend_function_entry class_MongoDB_Driver_BulkWriteCommand_methods[] = {
	ZEND_ME(MongoDB_Driver_BulkWriteCommand, __construct, arginfo_class_MongoDB_Driver_BulkWriteCommand___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_BulkWriteCommand, count, arginfo_class_MongoDB_Driver_BulkWriteCommand_count, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_Driver_BulkWriteCommand, deleteOne, arginfo_class_MongoDB_Driver_BulkWriteCommand_deleteOne, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_BulkWriteCommand, deleteMany, arginfo_class_MongoDB_Driver_BulkWriteCommand_deleteMany, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_BulkWriteCommand, insertOne, arginfo_class_MongoDB_Driver_BulkWriteCommand_insertOne, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_BulkWriteCommand, replaceOne, arginfo_class_MongoDB_Driver_BulkWriteCommand_replaceOne, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_BulkWriteCommand, updateOne, arginfo_class_MongoDB_Driver_BulkWriteCommand_updateOne, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_BulkWriteCommand, updateMany, arginfo_class_MongoDB_Driver_BulkWriteCommand_updateMany, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_BulkWriteCommand(zend_class_entry *class_entry_Countable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "BulkWriteCommand", class_MongoDB_Driver_BulkWriteCommand_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;
	zend_class_implements(class_entry, 1, class_entry_Countable);

	return class_entry;
}
