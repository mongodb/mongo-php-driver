/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 7f5bb1818756794e7ac1d2fe75f1a0f3d8d710a1 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_CursorId___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_CursorId___set_state, 0, 1, MongoDB\\Driver\\CursorId, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_CursorId___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_CursorId_serialize arginfo_class_MongoDB_Driver_CursorId___toString

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_CursorId_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_CursorId_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_CursorId___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_CursorId___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_CursorId, __construct);
static ZEND_METHOD(MongoDB_Driver_CursorId, __set_state);
static ZEND_METHOD(MongoDB_Driver_CursorId, __toString);
static ZEND_METHOD(MongoDB_Driver_CursorId, serialize);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_Driver_CursorId, unserialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_Driver_CursorId, unserialize);
#endif
static ZEND_METHOD(MongoDB_Driver_CursorId, __unserialize);
static ZEND_METHOD(MongoDB_Driver_CursorId, __serialize);


static const zend_function_entry class_MongoDB_Driver_CursorId_methods[] = {
	ZEND_ME(MongoDB_Driver_CursorId, __construct, arginfo_class_MongoDB_Driver_CursorId___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_CursorId, __set_state, arginfo_class_MongoDB_Driver_CursorId___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_CursorId, __toString, arginfo_class_MongoDB_Driver_CursorId___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_CursorId, serialize, arginfo_class_MongoDB_Driver_CursorId_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_Driver_CursorId, unserialize, arginfo_class_MongoDB_Driver_CursorId_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_Driver_CursorId, unserialize, arginfo_class_MongoDB_Driver_CursorId_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_Driver_CursorId, __unserialize, arginfo_class_MongoDB_Driver_CursorId___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_CursorId, __serialize, arginfo_class_MongoDB_Driver_CursorId___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_CursorId(zend_class_entry *class_entry_Serializable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "CursorId", class_MongoDB_Driver_CursorId_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 1, class_entry_Serializable);

	return class_entry;
}
