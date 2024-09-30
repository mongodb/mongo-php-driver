/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: c2927c5d481f305b1579a8968eeecc39ba921ddd */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Cursor___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_MongoDB_Driver_Cursor_current, 0, 0, MAY_BE_ARRAY|MAY_BE_OBJECT|MAY_BE_NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_OBJ_TYPE_MASK_EX(arginfo_class_MongoDB_Driver_Cursor_getId, 0, 0, MongoDB\\Driver\\CursorId|MongoDB\\BSON\\Int64, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, asInt64, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Cursor_getServer, 0, 0, MongoDB\\Driver\\Server, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Cursor_isDead, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Cursor_key, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Cursor_next, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Cursor_rewind arginfo_class_MongoDB_Driver_Cursor_next

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Cursor_setTypeMap, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, typemap, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Cursor_toArray, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Cursor_valid arginfo_class_MongoDB_Driver_Cursor_isDead


static ZEND_METHOD(MongoDB_Driver_Cursor, __construct);
static ZEND_METHOD(MongoDB_Driver_Cursor, current);
static ZEND_METHOD(MongoDB_Driver_Cursor, getId);
static ZEND_METHOD(MongoDB_Driver_Cursor, getServer);
static ZEND_METHOD(MongoDB_Driver_Cursor, isDead);
static ZEND_METHOD(MongoDB_Driver_Cursor, key);
static ZEND_METHOD(MongoDB_Driver_Cursor, next);
static ZEND_METHOD(MongoDB_Driver_Cursor, rewind);
static ZEND_METHOD(MongoDB_Driver_Cursor, setTypeMap);
static ZEND_METHOD(MongoDB_Driver_Cursor, toArray);
static ZEND_METHOD(MongoDB_Driver_Cursor, valid);


static const zend_function_entry class_MongoDB_Driver_Cursor_methods[] = {
	ZEND_ME(MongoDB_Driver_Cursor, __construct, arginfo_class_MongoDB_Driver_Cursor___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Cursor, current, arginfo_class_MongoDB_Driver_Cursor_current, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_Driver_Cursor, getId, arginfo_class_MongoDB_Driver_Cursor_getId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Cursor, getServer, arginfo_class_MongoDB_Driver_Cursor_getServer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Cursor, isDead, arginfo_class_MongoDB_Driver_Cursor_isDead, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Cursor, key, arginfo_class_MongoDB_Driver_Cursor_key, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_Driver_Cursor, next, arginfo_class_MongoDB_Driver_Cursor_next, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_Driver_Cursor, rewind, arginfo_class_MongoDB_Driver_Cursor_rewind, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_Driver_Cursor, setTypeMap, arginfo_class_MongoDB_Driver_Cursor_setTypeMap, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Cursor, toArray, arginfo_class_MongoDB_Driver_Cursor_toArray, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Cursor, valid, arginfo_class_MongoDB_Driver_Cursor_valid, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Cursor(zend_class_entry *class_entry_Iterator, zend_class_entry *class_entry_MongoDB_Driver_CursorInterface)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Cursor", class_MongoDB_Driver_Cursor_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;
	zend_class_implements(class_entry, 2, class_entry_Iterator, class_entry_MongoDB_Driver_CursorInterface);

	return class_entry;
}
