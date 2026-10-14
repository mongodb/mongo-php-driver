/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: df810b4d4876637e555a0e129e940acc5177b892 */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_MaxKey___set_state, 0, 1, MongoDB\\BSON\\MaxKey, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_MaxKey___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_MaxKey___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_MaxKey_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_BSON_MaxKey, __set_state);
static ZEND_METHOD(MongoDB_BSON_MaxKey, __unserialize);
static ZEND_METHOD(MongoDB_BSON_MaxKey, __serialize);
static ZEND_METHOD(MongoDB_BSON_MaxKey, jsonSerialize);


static const zend_function_entry class_MongoDB_BSON_MaxKey_methods[] = {
	ZEND_ME(MongoDB_BSON_MaxKey, __set_state, arginfo_class_MongoDB_BSON_MaxKey___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_MaxKey, __unserialize, arginfo_class_MongoDB_BSON_MaxKey___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_MaxKey, __serialize, arginfo_class_MongoDB_BSON_MaxKey___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_MaxKey, jsonSerialize, arginfo_class_MongoDB_BSON_MaxKey_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_MaxKey(zend_class_entry *class_entry_MongoDB_BSON_MaxKeyInterface, zend_class_entry *class_entry_JsonSerializable, zend_class_entry *class_entry_MongoDB_BSON_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "MaxKey", class_MongoDB_BSON_MaxKey_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 3, class_entry_MongoDB_BSON_MaxKeyInterface, class_entry_JsonSerializable, class_entry_MongoDB_BSON_Type);

	return class_entry;
}
