/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: f7c25df9cd8a81f89ac604821bae4a807d68ad78 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_CursorId___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_CursorId___set_state, 0, 1, MongoDB\\Driver\\CursorId, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_CursorId___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_CursorId_serialize arginfo_class_MongoDB_Driver_CursorId___toString

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_CursorId_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_CursorId___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_CursorId___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_CursorId, __construct);
static ZEND_METHOD(MongoDB_Driver_CursorId, __set_state);
static ZEND_METHOD(MongoDB_Driver_CursorId, __toString);
static ZEND_METHOD(MongoDB_Driver_CursorId, serialize);
static ZEND_METHOD(MongoDB_Driver_CursorId, unserialize);
static ZEND_METHOD(MongoDB_Driver_CursorId, __unserialize);
static ZEND_METHOD(MongoDB_Driver_CursorId, __serialize);


static const zend_function_entry class_MongoDB_Driver_CursorId_methods[] = {
	ZEND_ME(MongoDB_Driver_CursorId, __construct, arginfo_class_MongoDB_Driver_CursorId___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_CursorId, __set_state, arginfo_class_MongoDB_Driver_CursorId___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_CursorId, __toString, arginfo_class_MongoDB_Driver_CursorId___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_CursorId, serialize, arginfo_class_MongoDB_Driver_CursorId_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_CursorId, unserialize, arginfo_class_MongoDB_Driver_CursorId_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_CursorId, __unserialize, arginfo_class_MongoDB_Driver_CursorId___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_CursorId, __serialize, arginfo_class_MongoDB_Driver_CursorId___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_CursorId(zend_class_entry *class_entry_Serializable, zend_class_entry *class_entry_Stringable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "CursorId", class_MongoDB_Driver_CursorId_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 2, class_entry_Serializable, class_entry_Stringable);

	return class_entry;
}
