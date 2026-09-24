/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 497915f5aa3a10fc5e4bf222c82cdcb5c891a1bd */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_fromJSON, 0, 1, MongoDB\\BSON\\PackedArray, 0)
	ZEND_ARG_TYPE_INFO(0, json, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_fromPHP, 0, 1, MongoDB\\BSON\\PackedArray, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_get, 0, 1, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_getIterator, 0, 0, MongoDB\\BSON\\Iterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_has, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_MongoDB_BSON_PackedArray_toPHP, 0, 0, MAY_BE_ARRAY|MAY_BE_OBJECT)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, typeMap, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_toCanonicalExtendedJSON, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_PackedArray_toRelaxedExtendedJSON arginfo_class_MongoDB_BSON_PackedArray_toCanonicalExtendedJSON

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_offsetExists, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_offsetGet, 0, 1, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_offsetSet, 0, 2, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_offsetUnset, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_MIXED, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_PackedArray___toString arginfo_class_MongoDB_BSON_PackedArray_toCanonicalExtendedJSON

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray___set_state, 0, 1, MongoDB\\BSON\\PackedArray, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_BSON_PackedArray, __construct);
static ZEND_METHOD(MongoDB_BSON_PackedArray, fromJSON);
static ZEND_METHOD(MongoDB_BSON_PackedArray, fromPHP);
static ZEND_METHOD(MongoDB_BSON_PackedArray, get);
static ZEND_METHOD(MongoDB_BSON_PackedArray, getIterator);
static ZEND_METHOD(MongoDB_BSON_PackedArray, has);
static ZEND_METHOD(MongoDB_BSON_PackedArray, toPHP);
static ZEND_METHOD(MongoDB_BSON_PackedArray, toCanonicalExtendedJSON);
static ZEND_METHOD(MongoDB_BSON_PackedArray, toRelaxedExtendedJSON);
static ZEND_METHOD(MongoDB_BSON_PackedArray, offsetExists);
static ZEND_METHOD(MongoDB_BSON_PackedArray, offsetGet);
static ZEND_METHOD(MongoDB_BSON_PackedArray, offsetSet);
static ZEND_METHOD(MongoDB_BSON_PackedArray, offsetUnset);
static ZEND_METHOD(MongoDB_BSON_PackedArray, __toString);
static ZEND_METHOD(MongoDB_BSON_PackedArray, __set_state);
static ZEND_METHOD(MongoDB_BSON_PackedArray, __unserialize);
static ZEND_METHOD(MongoDB_BSON_PackedArray, __serialize);


static const zend_function_entry class_MongoDB_BSON_PackedArray_methods[] = {
	ZEND_ME(MongoDB_BSON_PackedArray, __construct, arginfo_class_MongoDB_BSON_PackedArray___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(MongoDB_BSON_PackedArray, fromJSON, arginfo_class_MongoDB_BSON_PackedArray_fromJSON, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, fromPHP, arginfo_class_MongoDB_BSON_PackedArray_fromPHP, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, get, arginfo_class_MongoDB_BSON_PackedArray_get, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, getIterator, arginfo_class_MongoDB_BSON_PackedArray_getIterator, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, has, arginfo_class_MongoDB_BSON_PackedArray_has, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, toPHP, arginfo_class_MongoDB_BSON_PackedArray_toPHP, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, toCanonicalExtendedJSON, arginfo_class_MongoDB_BSON_PackedArray_toCanonicalExtendedJSON, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, toRelaxedExtendedJSON, arginfo_class_MongoDB_BSON_PackedArray_toRelaxedExtendedJSON, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, offsetExists, arginfo_class_MongoDB_BSON_PackedArray_offsetExists, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_BSON_PackedArray, offsetGet, arginfo_class_MongoDB_BSON_PackedArray_offsetGet, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_BSON_PackedArray, offsetSet, arginfo_class_MongoDB_BSON_PackedArray_offsetSet, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_BSON_PackedArray, offsetUnset, arginfo_class_MongoDB_BSON_PackedArray_offsetUnset, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_BSON_PackedArray, __toString, arginfo_class_MongoDB_BSON_PackedArray___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, __set_state, arginfo_class_MongoDB_BSON_PackedArray___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, __unserialize, arginfo_class_MongoDB_BSON_PackedArray___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, __serialize, arginfo_class_MongoDB_BSON_PackedArray___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_PackedArray(zend_class_entry *class_entry_IteratorAggregate, zend_class_entry *class_entry_ArrayAccess, zend_class_entry *class_entry_MongoDB_BSON_Type, zend_class_entry *class_entry_Stringable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "PackedArray", class_MongoDB_BSON_PackedArray_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 4, class_entry_IteratorAggregate, class_entry_ArrayAccess, class_entry_MongoDB_BSON_Type, class_entry_Stringable);

	return class_entry;
}
