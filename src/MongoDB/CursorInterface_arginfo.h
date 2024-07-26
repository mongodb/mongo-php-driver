/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 95f356a63692e1bb72d50c641e93a53113c43531 */

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_OBJ_TYPE_MASK_EX(arginfo_class_MongoDB_Driver_CursorInterface_getId, 0, 0, MongoDB\\Driver\\CursorId|MongoDB\\BSON\\Int64, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_CursorInterface_getServer, 0, 0, MongoDB\\Driver\\Server, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_CursorInterface_isDead, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_CursorInterface_setTypeMap, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, typemap, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_CursorInterface_toArray, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()




static const zend_function_entry class_MongoDB_Driver_CursorInterface_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_Driver_CursorInterface, getId, arginfo_class_MongoDB_Driver_CursorInterface_getId, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_Driver_CursorInterface, getServer, arginfo_class_MongoDB_Driver_CursorInterface_getServer, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_Driver_CursorInterface, isDead, arginfo_class_MongoDB_Driver_CursorInterface_isDead, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_Driver_CursorInterface, setTypeMap, arginfo_class_MongoDB_Driver_CursorInterface_setTypeMap, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_Driver_CursorInterface, toArray, arginfo_class_MongoDB_Driver_CursorInterface_toArray, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_CursorInterface(zend_class_entry *class_entry_Traversable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "CursorInterface", class_MongoDB_Driver_CursorInterface_methods);
	class_entry = zend_register_internal_interface(&ce);
	zend_class_implements(class_entry, 1, class_entry_Traversable);

	return class_entry;
}
